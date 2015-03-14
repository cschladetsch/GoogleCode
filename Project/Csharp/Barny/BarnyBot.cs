using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Threading;

using Barny.Irc;

namespace Barny
{
	public class BarnyBot : IBot
	{
		class MultiSend
		{
			public IChannel channel;
			public int chunk_size;
			public string text;
			public int id;

			public MultiSend(IChannel C, int N, int K, string T)
			{
				channel = C;
				chunk_size = K;
				text = T;
				id = N;
			}
		}

		#region Fields
		delegate void CommandHandler(IUser user, string text);
		Dictionary<string, CommandHandler> commands = new Dictionary<string, CommandHandler>();
		Dictionary<int, MultiMessageSink> message_sinks = new Dictionary<int, MultiMessageSink>();

		readonly Regex command_regex = new Regex(@"!(?<cmd>(\w+)) (?<args>(.*))");
		readonly StringNumberCodec number_codec = new StringNumberCodec();
		readonly Irc.IClient client;
		readonly StringNumberCodec text_codec = new StringNumberCodec();
		IChannel chat_channel, data_channel;
		int max_data_text_length = 400;
		#endregion

		BarnyWindow window;

		public BarnyBot(BarnyWindow window, IClient client, IChannel chat_channel, IChannel data_channel)
		{
			this.window = window;
			this.client = client;
			this.chat_channel = chat_channel;
			this.data_channel = data_channel;
			AddCommandHandlers();
		}

		public void SendClip(IClip clip)
		{
			if (clip.User == client.User)
				SendData(string.Format("!addclip '{0}' {1}", clip.Name, EncodeClip(clip.Contents)));
		}

		void SendData(string text)
		{
			if (text.Length > max_data_text_length)
				new Thread(MultipartSend).Start(new MultiSend(data_channel, ++GetLocalUser().next_message_id, max_data_text_length, Encode(text)));
			else
				client.SendText(data_channel, text);
		}

		private void MultipartSend(object Q)
		{
			MultiSend send = (MultiSend)Q;
			Debug.WriteLine(string.Format("MultiPartSend: {0} {1} {2}", send.id, send.chunk_size, send.text));
			string remaining = send.text;
			int num_parts = (send.text.Length + send.chunk_size) / send.chunk_size;
			for (int n = 0; n < num_parts; ++n)
			{
				string header = string.Format("!multi {0} {1} {2} ", send.id, n, num_parts);

				int length = remaining.Length;
				if (length > send.chunk_size)
					length = send.chunk_size;
				length -= header.Length;
				string load = remaining.Substring(0, length);
				remaining = remaining.Substring(length);
				client.SendText(data_channel, string.Format("{0}{1}", header, load));

				Thread.Sleep(300);
			}
		}

		User GetLocalUser()
		{
			return (User)client.User;
		}

		public bool Handle(IChannel channel, IUser from, string text)
		{
			if (string.IsNullOrEmpty(text) || text[0] != '!')
				return false;

			Match match = command_regex.Match(text);
			if (!match.Success)
				return false;

			string cmd = match.Groups["cmd"].ToString();
			if (!commands.ContainsKey(cmd))
				return false;

			string args = match.Groups["args"].ToString();
			commands[cmd](from, args);
			return true;
		}

		void AddCommandHandlers()
		{
			commands["points"] = BotAddPoints;
			commands["color"] = BotSetColor;
			commands["thick"] = BotSetPenThickness;
			commands["bitmap"] = BotPasteImage;
			commands["save"] = BotSaveImage;
			//commands["clear"] = BotClearImage;
			commands["addclip"] = BotAddClip;
			commands["multi"] = BotMultiMessage;
		}

		bool IsLocalUser(IUser from)
		{
			return false;
		}

		#region BotCommands

		void BotAddPoints(IUser from, string text)
		{
			if (IsLocalUser(from))
				return;

			//if (!pens.ContainsKey(from.nick))
			//    pens[from.nick] = new Pen(Color.Blue);

			//Pen pen = pens[from.nick];
			//Point A = StringToPoint(text.Substring(0, 2), text.Substring(2, 2));
			//for (int N = 1; N < text.Length / 4; ++N)
			//{
			//    Point B = StringToPoint(text.Substring(N * 4, 2), text.Substring(N * 4 + 2, 2));
			//    //image_graphics.DrawLine(pen, A, B);
			//    A = B;
			//}

			//pictureBox1.BeginInvoke(new VoidFunction(pictureBox1.Refresh));
		}

		void BotSetColor(IUser user, string text)
		{
			//if (!pens.ContainsKey(user.nick))
			//    pens[user.nick] = new Pen(Color.Black);
			//pens[user.nick] = new Pen(StringToColor(text), pens[user.nick].Width);
		}

		class MultiMessageSink
		{
			public int id;
			public int next_part = 0, num_parts;
			public StringBuilder result = new StringBuilder();

			public MultiMessageSink(int N, int K)
			{
				id = N;
				num_parts = K;
			}

			public bool Receive(string N)
			{
				result.Append(N);
				return ++next_part == num_parts;
			}
		}

		private void BotMultiMessage(IUser from, string args)
		{
			Debug.WriteLine("BotMultiMessage: '" + args + "'");
			Regex R = new Regex(@"(?<id>(\w+)) (?<part>(\w+)) (?<total>(\w+)) (?<load>(\S+))");
			Match M = R.Match(args);
			if (!M.Success)
			{
				MessageBox.Show("Multi failed: " + args);
				return;
			}
			Debug.WriteLine("ID = '" + M.Groups["id"].ToString() + "'");
			int id = int.Parse(M.Groups["id"].ToString());
			int part = int.Parse(M.Groups["part"].ToString());
			int total = int.Parse(M.Groups["total"].ToString());
			string load = M.Groups["load"].ToString();

			Debug.WriteLine(string.Format("MULTI: id={0} part={1} total={2} load={3}", id, part, total, load));

			if (!message_sinks.ContainsKey(id))
			{
				message_sinks[id] = new MultiMessageSink(id, total);
			}

			MultiMessageSink sink = message_sinks[id];
			if (sink.Receive(load))
			{
				string D = Decode(sink.result.ToString());
				Regex W = new Regex(@"\!(?<cmd>\w+) (?<args>(.*))");
				Match E = W.Match(D);
				if (!E.Success)
				{
					MessageBox.Show("Decode multi failed");
					return;
				}
				string cmd = E.Groups["cmd"].ToString();
				string args2 = E.Groups["args"].ToString();
				commands[cmd](from, args2);
			}
		}

		Color StringToColor(string S)
		{
			try
			{
				Regex R = new Regex(@"(?<R>(\w+)) (?<G>(\w*)) (?<B>(\w+))");
				Match M = R.Match(S);
				if (M.Success)
				{
					int r = int.Parse(M.Groups["R"].ToString());
					int g = int.Parse(M.Groups["G"].ToString());
					int b = int.Parse(M.Groups["B"].ToString());

					return Color.FromArgb(r, g, b);
				}
			}
			catch (Exception e)
			{
				MessageBox.Show("Failed to parse color from " + S + ":\n\n" + e, "Error Clearing Image", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
			return Color.FromArgb(255, 0, 255);
		}

		void BotSetPenThickness(IUser user, string text)
		{
			float thickness = float.Parse(text);
			// TODO: put pen in user
			string from = user.Name;
			//if (!pens.ContainsKey(from))
			//    pens[from] = new Pen(Color.Black);
			//pens[from] = new Pen(pens[from].Color, thickness);
		}

		void BotPasteImage(IUser user, string text)
		{
			try
			{
				Regex R = new Regex(@"(?<w>(\w+)) (?<h>(\w+)) (?<data>(.*))");
				Match M = R.Match(text);
				int w = int.Parse(M.Groups["w"].ToString());
				int h = int.Parse(M.Groups["h"].ToString());
				string data = M.Groups["data"].ToString();

				Bitmap bitmap = new Bitmap(w, h);
				int C = 0;
				for (int Y = 0; Y < h; ++Y)
				{
					for (int X = 0; X < w; ++X)
					{
						int r = this.number_codec.Decode(data.Substring(C + 0, 2));
						int g = this.number_codec.Decode(data.Substring(C + 2, 2));
						int b = this.number_codec.Decode(data.Substring(C + 4, 2));
						C += 6;

						bitmap.SetPixel(X, Y, Color.FromArgb(r, g, b));
					}
				}

				//image_graphics.DrawImage(bitmap, 0, 0); TODO
				//pictureBox1.Refresh();
			}
			catch (Exception e)
			{
				MessageBox.Show(e.ToString(), "Exception with Bitmap", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
			}
		}

		Regex clip_regex = new Regex(@"'(?<title>.*)' (?<data>\S+)");
		delegate void AddClipFunction(IClip clip);

		private void BotAddClip(IUser user, string args)

		{
			Match match = clip_regex.Match(args);
			if (!match.Success)
			{
				Debug.WriteLine("BotAddClip: FAILED to match '" + args + "'");
				return;
			}

			string clip_data = match.Groups["data"].ToString();
			string clip_title = match.Groups["title"].ToString();
			IDataObject text = DecodeClip(clip_data);
			string title = clip_title;
			Clip clip = new Clip(user, title, text);
			Database.Clips.Add(clip);

			window.BeginInvoke(new AddClipFunction(window.NewClip), new object[] { clip });
			
			//client.SendText(chat_channel, text);
		}

		void BotSaveImage(IUser user, string text)
		{
			//TODO BeginInvoke(new VoidFunction(SaveImage));
		}

		void BotUploadRequest(IUser user, string args)
		{
			Regex R = new Regex(@"(?<filename>(\w+)) (?<size>(\w+))");
			Match M = R.Match(args);
			if (!M.Success)
			{
				MessageBox.Show("Bad upload request '" + args + "' from " + user.Name, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			int size = int.Parse(M.Groups["size"].ToString());
			string filename = M.Groups["filename"].ToString();
			string text = string.Format("Accept file {0} ({1}kb) from {2}?", filename, size / 1024, user.Name);
			if (MessageBox.Show(text, "Upload request", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.OK)
			{
				//client.SendText(string.Format("!accept {0} {1}", user.Name, args));
				//downloader = new Downloader(user.ip_address, 6666, size, filename);
			}
		}
		#endregion

		#region Codec
		private string Decode(string text)
		{
			StringBuilder decoded = new StringBuilder();
			for (int N = 0; N < text.Length / 2; ++N)
				decoded.Append((char)text_codec.Decode(text.Substring(N * 2, 2)));
			return decoded.ToString();
		}


		public void AddClip(string title, IDataObject clip)
		{
			string t = Encode(title), b = EncodeClip(clip);
			//TODO SendTextBoth(string.Format("!addclip {0} {1}", t, b));
		}

		private string EncodeClip(IDataObject Q)
		{
			if (Q.GetDataPresent(DataFormats.Html, false))
				return Encode("HTML" + Q.GetData(DataFormats.Html).ToString());
			if (Q.GetDataPresent(DataFormats.Rtf, true))
				return Encode("RTF " + Q.GetData(DataFormats.Rtf).ToString());
			if (Q.GetDataPresent(DataFormats.Text, true))
				return Encode("TEXT" + Q.GetData(DataFormats.Text).ToString());
			//if (Q.GetDataPresent(DataFormats.Bitmap, true))
			//    return Encode("BMP " + Q.GetData(DataFormats.Text).ToString());

			return Encode("NONE");
		}

		string EncodeBytes(byte[] bytes)
		{
			return number_codec.EncodeByteArray(bytes);
		}

		byte[] DecodeBytes(string S)
		{
			return number_codec.DecodeByteArray(S);
		}

		private IDataObject DecodeClip(string S)
		{
			string D = Decode(S);//new string(Decompress(DecodeBytes(S))_;// DecodeBytes(Decompress(S));
			string type = D.Substring(0, 4);
			D = D.Substring(4);
			//MessageBox.Show("DECODED STRING = " + D);
			if (type == "HTML")
				return new DataObject(DataFormats.Html, D);
			if (type == "RTF ")
				return new DataObject(DataFormats.Rtf, D);
			if (type == "TEXT")
				return new DataObject(DataFormats.Text, D);
			return null;
		}

		private string Encode(byte[] text)
		{
			return text_codec.EncodeByteArray(text);
		}

		private string Encode(IEnumerable<char> text)
		{
			StringBuilder encoded = new StringBuilder();
			foreach (char C in text)
			{
				encoded.Append(text_codec.Encode(C));
			}
			return encoded.ToString();
		}

		#endregion

	}
}

//EOF
