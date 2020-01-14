import QtQuick 2.0
import Sailfish.Silica 1.0
import "utils.js" as Utils

ListItem {
    id: gameDelegate
    width: gameList.width
    contentHeight: Theme.itemSizeExtraLarge

    onClicked: {
        appWindow.viewChanged(gameid);
        pageStack.push(eventsPage);
    }

    // Label containing the total score
    Label {
        id: totalScore
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: periodsScore.top
        }
        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeMedium
        }
        color: gameDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor
        text: totalscore
    }

    // Label containing the per-period score
    Label {
        id: periodsScore
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: homeLogo.verticalCenter
        }
        font.pixelSize: Theme.fontSizeSmall
        color: gameDelegate.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
        text: "(" + periodsscore + ")"
    }

    // Label indicating the status of the game
    Label {
        id: gameStatus
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: periodsScore.bottom
        }
        font.pixelSize: Theme.fontSizeSmall
        color: gameDelegate.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
        text: gamestatus
    }

    // Hometeam logo: to the left
    Image {
        id: homeLogo
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
        }
        height: 96
        width: 96
        fillMode: Image.PreserveAspectFit
        source: "../icons/" + model.hometeamId + ".png"
        onStatusChanged: Utils.checkIcon(homeLogo, hometeamId)
    }

    // Awayteam logo: to the right
    Image {
        id: awayLogo
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }
        height: 96
        width: 96
        fillMode: Image.PreserveAspectFit
        source: "../icons/" + model.awayteamId + ".png"
        onStatusChanged: Utils.checkIcon(awayLogo, awayteamId)
    }
}
