
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "v8gl.h"
#include "../lib/fs.h"



int main(int argc, char* argv[]) {

	char usage[] = "\n\
lycheeJS V8ADK runtime\n\
\n\
Recommended development environment: Ubuntu 12.04 64Bit\n\
\n\
The V8ADK runtime allows parsing the game, its environment and its\n\
behaviours and exports the parsed data to the lycheeJS ADK.\n\
\n\
\n\
Usage: %s <task> <path/to/init.js>\n\
\n\
\n\
Tasks:\n\
	execute          Executes the JavaScript file.\n\
	export-bash      Exports the environment to a via-bash includable format.\n\
	export-json      Exports the environment to a JSON format.\n\
	rewrite          Rewrites and prepares your init.js.\n\
	rewrite-v8gl     Rewrites and prepares your init.js to fit into the V8GL runtime.\n\
\n\
\n\
Examples:\n\
\n\
v8adk export-bash ./external/lycheeJS/game/boilerplate/init.js > game_env.sh\n\
v8adk export-json ./external/lycheeJS/game/boilerplate/init.js > game_env.json\n\
v8adk rewrite-v8gl ./external/lycheeJS/game/boilerplate/init.js > v8gl_init.js\n\
\n\
";

	if (argc == 3) {

		char buf[PATH_MAX + 1];
		char *path = realpath(argv[2], buf);

		if (path) {

			char* rawsource = lib::FS::read(path);


			v8::HandleScope scope;
			v8::Persistent<v8::Context> context = v8gl::V8GL::initialize(&argc, argv);


			if (strcmp(argv[1], "export-bash") == 0) {
				v8gl::V8GL::dispatch(context, (char*) "export-bash");
			} else if (strcmp(argv[1], "export-json") == 0) {
				v8gl::V8GL::dispatch(context, (char*) "export-json");
			} else if (strcmp(argv[1], "rewrite-v8gl") == 0) {
				v8gl::V8GL::dispatch(context, (char*) "rewrite-v8gl");
			} else if (strcmp(argv[1], "rewrite") == 0) {
				v8gl::V8GL::dispatch(context, (char*) "rewrite");
			} else if (strcmp(argv[1], "execute") == 0) {
				v8gl::V8GL::dispatch(context, (char*) "execute");
			} else {

				fprintf(stderr, usage, argv[0]);
				return 1;

			}


			v8::Local<v8::String> source = v8::String::New(rawsource);
			if (source.IsEmpty()) {
				fprintf(stderr, "Error reading file %s", path);
			} else {

				lib::FS::setVMRoot(argv[0], argv[2]);

				v8gl::V8GL::execute(context, source, v8::String::New(path));

			}


			delete rawsource;

			context.Dispose();

			return 0;

		} else {

			fprintf(stderr, usage, argv[0]);
			return 1;

		}

	} else {

		fprintf(stderr, usage, argv[0]);
		return 1;

	}

}

