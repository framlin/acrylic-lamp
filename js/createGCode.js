var fs = require('fs');

var argv = require('minimist')(process.argv.slice(2)),
    fileName = null;

var gcoder = require("gcoder");

const PLATE_LENGTH = 150;
const PLATE_WIDTH = 6;
const SLOT_DEPTH = 10;
const SLOT_TOOL_DIAMETER = 1.5875;
const SLOT_FEEDRATE = 800;

const X_SLOT_1 = 100;
const Y_SLOT_1 = 200;

var toolPath = ["G21", "G90", "G94"];//mm, absolute, feedrate per minute
//header
toolPath.push("F" + SLOT_FEEDRATE);
toolPath.push("G00 Z10.0000");
toolPath.push("M03"); //spindle start
toolPath.push("G04 P10"); //dwell 10ms

//toolPath.push("G01X"+X_SLOT_1+"Y"+Y_SLOT_1+"Z0");
toolPath.push("G10 L2 P2 X100 Y100 Z0");
toolPath.push("G55");
toolPath.push("G00 Z10.0000");


var pocketPath = gcoder.createPocket(PLATE_LENGTH, PLATE_WIDTH, SLOT_DEPTH, SLOT_TOOL_DIAMETER, SLOT_FEEDRATE);

toolPath = toolPath.join('\n') + pocketPath;

function dump_gcode(filePath, gcode) {
    if (filePath) {
        fs.writeFile(filePath, gcode, function(err) {
            if(err) {
                return console.log(err);
            }

            console.log("The g-code-file was saved!");
        });
    } else {
        console.log(gcode);
    }

}

function dumpHelp() {
    console.log("usage: node create-pocket-gcode -l x -w x -d x -t x -f x -o fn");
    console.log("=====> l: length, w: width, d: depth, t: toolDiameter, f: feed rate o: fileName");
    console.log();
    console.log("creates the gCode tool-path for a pocket with the given params");
    console.log("center the tool at      X: 0 + t/2     Y: 0 + t/2     Z: 0");
    console.log();
}

dump_gcode(null, toolPath);
/*
if (argv.h) {
    dumpHelp();
} else {
    if (argv.l && argv.w && argv.d && argv.t && argv.f) {
        if (argv.o) {
            fileName = argv.o;
        }
        dump_gcode(fileName, createPath(argv.l, argv.w, argv.d, argv.t, argv.f));
    } else {
        dumpHelp();
    }
}
*/