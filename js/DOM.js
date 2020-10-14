/**
 * On start, set flash checkbox and available device ports
 */
$(document).ready(function() {
    //set flash checkbox unchecked
    $('#flash').prop('checked', true);
    //get ports list
    var command = 'arduino-cli board list';
    result = cp.exec(command, []);
    //manage ports obtained
    result.stdout.on('data', function(data) {
        data.toString().split('\n').forEach(element => {
            if (element.includes("/dev/")) {
                var port = element.split(' ')[0];
                $("#serial_port").append(new Option(port, port));
            }
        });
    }.bind(this));

});

/**
 * Action triggered when submit-button is clicked
 */
$('#build_compile_button').on('click', function() {
    build_compile_flash();
});

/**
 * Action trtiggered when hide-show button is pressed.
 */
$("#hide_show").on('click', function() {
    hide_show_terminal();
});

$("#flash").change(function() {
    if ($(this).is(":checked")) {
        $("#serial_port").prop("disabled", false);
    } else {
        $("#serial_port").prop("disabled", true);
    }
});

/**
 * Hides terminal if shown and shows the terminal if hidden
 * @param {boolean} force if true forces opening 
 */
var hide_show_terminal = function(force) {
    if ($('#terminal_holder').is(":hidden") || force) {
        //show terminal
        $('#terminal_holder').show();
        $('#content').css("bottom", "300px");
        $('#footer').css("height", "300px");
    }
    else {
        $('#terminal_holder').hide();
        $('#content').css("bottom", "30px");
        $('#footer').css("height", "30px");
    }
}

$("#save").on('click', function() {
    var zip = JSZip();
    zip.file('td.json', JSON.stringify(editor.getValue()));
    zip.file('build.json', JSON.stringify(builder.getValue()));

    dialog.showSaveDialog(options = {
        title: "Save embeddedWoTServient file",
        defaultPath: "./.wotsrv",
        filters: [
            {name: 'WoT Servient', extensions:['wot', 'wotsrv']},
            {name: 'Zip file', extensions:['zip']}
        ]
    }).then(result => {
        if (result.canceled) return;
        zip .generateNodeStream({type: 'nodebuffer', streamFiles: true})
            .pipe(fs.createWriteStream(result.filePath))
            .on('finish', function() {
                console.log("file saved");
            });
    });
});

$("#load").on('click', function() {
    dialog.showOpenDialog(options = {
        title: "Open embeddedWoTServient file",
        defaultPath: ".",
        filters: [
            {name: 'WoT Servient', extensions:['wot', 'wotsrv']},
            {name: 'Zip file', extensions:['zip']}
        ]
    }).then(result => {
        fs.readFile(result.filePaths[0], function(err, data) {
            // if (err) throw err;
            JSZip.loadAsync(data).then(function(zip) {
                    zip.file('td.json').async("string").then(function (data) {
                        // editor.setValue(JSON.parse(data));
                        editor.destroy();
                        createEditor(JSON.parse(data));
                    });

                    zip.file('build.json').async("string").then(function (data) {
                        // builder.setValue(JSON.parse(data));
                        builder.destroy();
                        createBuilder(JSON.parse(data));
                    });

                });
        });
    });
});