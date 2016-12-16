import QtQuick 2.0
import Sailfish.Silica 1.0

import "utils.js" as Utils

Page {
    property alias leagueName: pulleyMenu.leagueName

    function startUpdateIndicator() {
//        updateIndicator.running = true;
//        updateIndicator.visible = true;
    }

    function stopUpdateIndicator() {
//        updateIndicator.running = false;
//        updateIndicator.visible = false;
    }

    LeagueSelectionDialog {
        id: leagueSelectionDialog
    }

/*    // A spinning busy indicator shown while the data is loading upon
    // application start
    BusyIndicator {
        id: updateIndicator
        anchors {
            right: selectorIcon.left
            rightMargin: 32
            verticalCenter: parent.verticalCenter
        }

        platformStyle: BusyIndicatorStyle {
            size: "small"
            inverted: true
        }
        visible: false
        running: false
        //running: gameList.count == 0
        //visible: gameList.count == 0
    }*/

//    property bool bannerStart: false

    SilicaListView {
        id: gameList
        model: listData
        anchors.fill: parent

        delegate: ListItem {
            id: gameDelegate
            width: gameList.width
            contentHeight: Theme.itemSizeExtraLarge

            onClicked: {
                appWindow.viewChanged(gameid);
                pageStack.push(detailsPage);
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
                source: "../../icons/" + hometeamId + ".png"
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
                source: "../../icons/" + awayteamId + ".png"
                onStatusChanged: Utils.checkIcon(awayLogo, awayteamId)
            }
        }

        VerticalScrollDecorator{}

        PullDownMenu {
            id: pulleyMenu
            property string leagueName: "All"

            MenuItem {
                text: qsTr("Update")
                onClicked: appWindow.updateTriggered()
            }
            MenuItem {
                text: qsTr("League: ") + leagueName
                onClicked: {
                    pageStack.push(leagueSelectionDialog)
                }
            }
        }
    }

    // TODO: This should actually be inside the ListView, but that will place it
    // strangely so we put it here and take the error.
    ViewPlaceholder{
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        enabled: gameList.count === 0
        text: qsTr("No games today.");
        //hintText: qsTr("")
    }
}
