using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Window
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string []args)
		{
			_args = args;
			StartPath = System.IO.Directory.GetCurrentDirectory();
			
			System.IO.Directory.SetCurrentDirectory(@"..\..");
			
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			var thread = new System.Threading.Thread(RunBoxplorer);
			thread.Start();

			Application.Run(new Form1());
			thread.Abort();
		}

		public static BoxPlorer BoxPlorer;
		private static string[] _args;
		public static string StartPath;

		static void RunBoxplorer()
		{
			BoxPlorer = new BoxPlorer();
			//Bif (_args.Length > 0)
				//BoxPlorer.Start(_args[0]);
			Debug.Print(System.IO.Directory.GetCurrentDirectory());
			BoxPlorer.Start(@"cfgs\boxplorer.cfg");
		}
	}
}
