

// Remote URL: http://www.sihf.ch/Image/Club/<ID>.png?w=256&h=256
function checkIcon(file, teamId) {
    var remoteIcon = "http://www.sihf.ch/Image/Club/" + teamId + ".png?w=256&h=256";

    if(file.status === Image.Error && file.source !== remoteIcon) {
        file.source = remoteIcon
    }
}

