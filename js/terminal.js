const {Terminal} = require(path.resolve('node_modules/xterm/lib/xterm.js'));

var CustomTerminal = function () { 

    this.terminal = new Terminal(options = {
        cols: 150
    });
    this.terminal.open(document.getElementById('terminal_holder'));
    this.terminal.setOption('fontSize', 10)
    this.terminal.write('WoTServient$ ');

    //handle stdin
    this.terminal.onKey( (key, ev) => {
        this.stdLine += key.key;
        this.terminal.write(key.key);
        if (key.key.charCodeAt(0) == 13) {
            this.stdLine += '\n';
            this.cmd.stdin.write(this.stdLine);
            this.stdLine = '';
        }
        else if (key.key.charCodeAt(0) == 127) {
            this.terminal.write("\b \b");
            this.stdLine = this.stdLine.slice(0, -1);
        }
    })
}

CustomTerminal.prototype.write = function(message) {
    message.toString().split('\n').forEach(element => {
        this.terminal.writeln(element);
    });
    //this.terminal.writeln(message);
}

CustomTerminal.prototype.exec = function(command) {
    this.cmd = cp.exec(command, []);

    //handle stdout
    this.cmd.stdout.on('data', function(data) {
        //console.log(data.toString())
        data.toString().split('\n').forEach(element => {
            this.terminal.writeln(element);
        });
    }.bind(this));

    //handle stderr
    this.cmd.stderr.on('data', function(data) {
        //console.log(data.toString())
        data.toString().split('\n').forEach(element => {
            this.terminal.writeln('\x1B[1;3;31m'+element+'\x1B[0m');
        });
    }.bind(this));

    this.stdLine = '';
    
    //on exit
    this.cmd.on('exit', (code) => {
        this.terminal.writeln('Child process exited with exit code '+code);
        this.cmd.kill();
        this.cmd = undefined;
    });
}
