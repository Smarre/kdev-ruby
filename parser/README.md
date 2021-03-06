# Parser

## Intro

This is a bison-generated pure LALR parser. It's based on the MRI parser.
This parser has been designed to be small, simple and yet powerful. Its code
resides in four different files: parser.y, node.h, node.c and main.c.
The main.c file is conceived only for testing purposes (see the section
"Testing and Debugging the parser"). More important are the files node.h and
node.c that, in short, define what is a node and what can we (and the parser)
do with nodes. Finally, the parser.y file is the grammar of the parser. This
is the parser in its very basic shape. To see what this parser really looks
like, we have to generate three files: parser.h, parser.c and hash.c.
The parser\_gen.{h, c} files are generated by bison taking the grammar file as
its input. The last, (but not least) file to generate is hash.c and it's
generated by gperf taking the file tools/gperf.txt as its input. It contains
a hash table that is used by the parser to match keywords quickly. The
parser\_gen.{h, c} files are generated by cmake. The hash.c file is generated
with the tools/gperf.rb script.

## Testing and Debugging the Parser

All the info on testing the parser can be found [here](http://techbase.kde.org/Projects/KDevelop4/Ruby#Testing).

What represents all those integers from the output ? In short,
it's the representation of an AST printed in pre-order. As you
will see, the parser tries to beautify this output by telling you if the
expression is a condition inside of, for example, a for statement, or it will
output "Root" and "Next" if there is a list of inner statements. Moreover, the
parser sometimes outputs names between parenthesis. Those names are variables,
the name of a function, a class, etc. Sadly, sometimes the output is scary
and a complete mess. In those cases, experience and patience
will be our friends ;)

## Character encodings

As stated before, this parser is meant to be simple and small. This means
that by now we only support UTF-8 encoding. This doesn't mean that
other encodings will never be supported by this parser, it's just that
the developers haven't had enough time to write the code.


TODO: what to do with followinG; the techbase page does not exist any more.


 How to compile

First of all, you need to compile KDevelop and KDevPlatform. There are instructions on how to do this here: KDevelop4/HowToCompile. If you have succeeded in installing KDevelop and KDevPlatform, you can install the Ruby plugin in pretty much the same way. First, download the source code:

   git clone git://anongit.kde.org/kdev-ruby

Then you can compile it by doing the following:

   mkdir build && cd build
   cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/install/directory
   make
   make install

Testing
Testing manually

First of all, go to your build directory, all the testing has to be done in this directory. Now, there are two mechanisms to test this plugin: RSpec and QtTest. RSpec is used to test the parser, and QtTest to test everything else. If you want to test the parser, you'll have to run the following commands:

   cd parser/tools
   rspec -c -f doc parser_spec.rb

For the other components, there is an executable file for each of them. You just have to go inside its test directory and execute the tests. For example, to test the DUChain:

   cd duchain/tests
   ./duchain

Testing using Guard

All this process can be automated by using the guard gem. Note that there is a Gemfile in the source code of this plugin. So, to get guard and its dependencies, you may want to run the bundle command. And finally, in the root of the source tree you will have to execute the following command:

   guard -w /relative/path/to/the/build/directory

Now all the tests will be run and you'll get notified. Moreover, if you install the plugin again, all tests will be executed automatically.
Generating the documentation

This plugin needs to have a file with all the Ruby built-in modules, classes, constants, etc. This file is called builtins.rb and you can find it in the documentation/ directory. This file will always be available, but if you want to generate it again, you have to follow some simple steps. First of all, you need to get the Ruby source code (the MRI implementation). You can download it here or, alternatively, you can use svn or git like this:

   svn co http://svn.ruby-lang.org/repos/ruby/trunk ruby
   git clone https://github.com/ruby/ruby.git

This is a necessary step because the documentation/kdevruby_doc.rb script needs the path of the Ruby source code as the first parameter. The second parameter to be passed to this script will be the name of the output file. Therefore, to generate the builtins.rb file we just have to execute:

   cd documentation
   ruby kdevruby_doc.rb /absolute/path/to/the/ruby/source/code builtins.rb

Note that this process can take a while. After this, we'll have a brand new builtins.rb file.