// FXParser
//
// Reads HLSL .fx file, parses it, then walks a tree to re-write the file in HLSL format
//
// See Options class for command-line arguments
//
// (C) 2009-2010 Christian.Schladetsch@gmail.com
//

using System;
using System.Text;
using System.IO;
using System.Reflection;

using Antlr.Runtime;
using Antlr.Runtime.Tree;
using Antlr.StringTemplate;
using Antlr.StringTemplate.Language;

using Plossum.CommandLine;

namespace FXParser
{
    /// <summary>
    /// parse a HLSL shader file, writing it back out again via walking the AST tree
    /// </summary>
	class FxParser
    {
        /// <summary>
        /// options system for the effect translator, based on Plossum
        /// </summary>
		[CommandLineManager(ApplicationName = "FXParser", Copyright = "", EnabledOptionStyles = OptionStyles.Group | OptionStyles.LongUnix)]
		[CommandLineOptionGroup("commands", Name = "Commands", Require = OptionGroupRequirement.ExactlyOne)]
		[CommandLineOptionGroup("options", Name = "Options")]
		class Options
		{
			[CommandLineOption(Name = "v", Aliases = "verbose", Description = "Produce verbose output", GroupId = "options")]
			public bool Verbose = false;

			[CommandLineOption(Name = "q", Aliases = "quiet", Description = "Produce no ouput", GroupId = "options")]
			public bool Quiet = false;

            [CommandLineOption(Name = "o", Aliases = "out_dir", Description = "Output directory", GroupId = "options")]
            public string OutDir = "./";

			[CommandLineOption(Name = "i", Aliases = "infile", Description = "Specify the file to parse", GroupId = "options", MinOccurs = 1)]
			public string InputFile = "";

			[CommandLineOption(Name = "f", Aliases = "fx", Description = "Parse HLSL", GroupId = "commands")]
			public bool ParseHLSL = false;

            [CommandLineOption(Name = "d", Aliases = "debug", Description = "Set debug level", GroupId = "options")]
            public int debug = 0;

			[CommandLineOption(Name = "h", Aliases = "help", Description = "Shows this help text", GroupId = "commands")]
			public bool Help = false;
		}

        string basename;                    // basename of input file
		Options options = new Options();    // command-line options
        bool force_local = false;//true;            // hack work-around for debugging from the IDE

        /// <summary>
        /// main entry point for the program process
        /// </summary>
        /// <param name="args"></param>
        /// <returns></returns>
		int Run(string[] args)
		{
            // hack work-around for inability to pass command-line arguments to program when
            // debugging from the IDE. TODO: fix this
            if (!force_local)
            {
                var cmdParser = new CommandLineParser(options);
                cmdParser.Parse();

                if (options.Help)
                {
                    Console.WriteLine(cmdParser.UsageInfo.ToString(78, false));
                    return 0;
                }
                else if (cmdParser.HasErrors)
                {
                    Console.WriteLine(cmdParser.UsageInfo.ToString(78, true));
                    return -1;
                }

                var asm = Assembly.GetExecutingAssembly();
                var parts = asm.FullName.Split(',');
                var version = parts[1];
                if (options.Verbose)
                {
                    Console.WriteLine(asm.FullName);
                }
            }
            else
            {
                // hard-code effects of command-line parameters for debugging from the IDE
                options.ParseHLSL = true;
                options.debug = 1;
                options.InputFile = "test.fx";
            }

            // open the input file, read contents
            string contents;
			try
			{
				 contents = new StreamReader(options.InputFile).ReadToEnd();
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);
				return -1;
			}

            basename = options.InputFile;
            if (string.IsNullOrEmpty(basename))
            {
                basename = System.IO.Path.GetFileName(options.InputFile);
                basename = System.IO.Path.ChangeExtension(basename, null);
            }

            // do the work
            if (options.ParseHLSL)
                return ParseHlslFile(contents);
            return -1;
        }

        // parse HLSL .fx file contents
        int ParseHlslFile(string contents)
        {
            var fx = LoadStringTemplates(Internal.Resource1.FXWrite);
			var errors = false;
            try
            {
				errors = !WriteFxTemplateResult(contents, fx, options.OutDir, basename, "out.fx");
            }
            catch (Antlr.Runtime.MismatchedTokenException e)
            {
				errors = true;
				Console.WriteLine(@"MismatchedTokenException : {0}", e.Message);
            }
            catch (Exception e)
            {
				errors = true;
                Console.WriteLine(@"Exception: {0}", e.Message);
            }
			return errors ? -1 : 0;
        }

        bool WriteFxTemplateResult(string contents, StringTemplateGroup string_templates, string dir, string basename, string ext)
        {
            var input = new ANTLRStringStream(contents);
            var lexer = new ShaderLexer(input);
            var tokens = new CommonTokenStream(lexer);
            var parser = new ShaderParser(tokens);

			try
			{
                var result = parser.fx_file();
				if (result == null)
				{
					Console.WriteLine("Error parsing HLSL");
					return false;
				}
                ITreeNodeStream nodes = new CommonTreeNodeStream(result.Tree);
                var tree = new ShaderTree(nodes) {TemplateLib = string_templates};
			    var result2 = tree.entry();

                if (result2 == null)
                {
                    Console.WriteLine("Error walking HLSL tree");
                    return false;
                }

				var filename = dir + basename + ext;
                WriteTextFile(filename, ReplaceStamps(result2.Template.ToString()));
				if (!options.Quiet)
					Console.Out.WriteLine("Wrote {0}", filename);
			}
			catch (Exception e)
			{
				Console.Out.WriteLine("Exception {0}", e.Message);
                return false;
			}
			return true;
		}

        /// <summary>
        /// pretty-print an abstract syntax tree
        /// </summary>
        /// <param name="text">output string</param>
        /// <param name="tree">tree to print</param>
        /// <param name="indent">indent level of current node in tree</param>
        static void PrettyPrint(StringBuilder text, CommonTree tree, int indent)
        {
            if (tree == null)
                return;
            const int Spacing = 2;
            text.Append('\n');
            text.Append(' ', indent * Spacing);
            text.Append(tree.Text);
            if (tree.Children == null) return;
            foreach (CommonTree child in tree.Children)
            {
                PrettyPrint(text, child, indent + 1);
            }
        }

        /// <summary>
        /// pretty-print an AST
        /// </summary>
        /// <param name="tree">the tree to print</param>
        void PrettyPrint(CommonTree tree)
        {
            var builder = new StringBuilder();
            PrettyPrint(builder, tree, 0);
            var text = builder.ToString();
            Console.WriteLine(text);
            System.Diagnostics.Debug.WriteLine(text);
        }

        /// <summary>
        /// replace format token-stamps in input text with live values
        /// </summary>
        /// <param name="text"></param>
        /// <returns>formatted string</returns>
        static string ReplaceStamps(string text)
        {
            text = text.Replace("[[DATE]]", DateTime.Now.ToShortDateString() + " " + DateTime.Now.ToShortTimeString());
            var name = Assembly.GetExecutingAssembly().GetName();
            var program = string.Format("{0} V{1}", name.Name, name.Version);
            text = text.Replace("[[PROGRAM]]", program);
            return text;
        }

        /// <summary>
        /// write a text file
        /// </summary>
        /// <param name="filename"></param>
        /// <param name="text"></param>
        static void WriteTextFile(string filename, string text)
        {
            using (TextWriter writer = new StreamWriter(filename))
            {
                writer.WriteLine("{0}", text);
                writer.Close();
            }
        }

        /// <summary>
        /// load string-templates from embedded resource
        /// </summary>
        /// <param name="resource"></param>
        /// <returns></returns>
        static StringTemplateGroup LoadStringTemplates(byte[] resource)
		{
			return new StringTemplateGroup(new StreamReader(new MemoryStream(resource)), typeof(AngleBracketTemplateLexer));
		}

        /// <summary>
        /// entry point for the application
        /// </summary>
        /// <param name="args"></param>
        /// <returns></returns>
		public static int Main(string[] args)
		{
			var translator = new FxParser();
			var result = translator.Run(args);
			if (result != 0)
				Console.ReadLine();
			return result;
		}
	}
}

//EOF
