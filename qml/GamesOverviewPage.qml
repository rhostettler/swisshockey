import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    tools: mainToolbar

    // The header
    Rectangle {
        id: header
        anchors {
            top: parent.top
            left: parent.left
        }
        width: parent.width
        height: 72
        z: 1
        color: "#FF0000"

        Text {
            id: leagueLabel
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 16
            }
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font {
                family: "Nokia Pure Light"
                pixelSize: 28
            }
            color: "#FFFFFF"
            text: "NL A"
            smooth: true
        }

        // Selector Icon
        Image {
            id: selectorIcon
            source: "icons/icon-m-toolbar-updown.png"
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 32
            }
            width: 13
            height: 24
            smooth: true
        }

        // Make it clickable using a MouseArea
        MouseArea {
            anchors.fill: parent
            onPressed: {
                header.color = "#800000";
            }
            onReleased: {
                header.color = "#FF0000";
            }
            onClicked: {
                leagueSelectionDialog.open();
            }
        }
    }

    // The selection dialog for choosing the filter
    // TODO: Here we need to implement all the action that should happen once a new selection is made.
    SelectionDialog {
         id: leagueSelectionDialog
         titleText: "Select League"
         selectedIndex: 0

         model: ListModel {
             ListElement { name: "NL A" }
/*             ListElement { name: "NL B" }
             ListElement { name: "Cup" }*/
         }
     }

    // A spinning busy indicator shown while the data is loading upon
    // application start
/*    BusyIndicator {
        id: spinningIndicator
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        platformStyle: BusyIndicatorStyle { size: "large" }
        running: gameList.count == 0
        visible: gameList.count == 0
    }*/
    Text {
        id: statusLabel
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font {
            family: "Nokia Pure Light"
            pointSize: 18
        }
        color: "#000000"
        text: "No games today."
        smooth: true
        visible: gameList.count == 0
    }

    // The delegate that draws the different games
   Component {
        id: gameDelegate

        Item{
            width: gameList.width
            height: 128

            /* TODO: Disabled since game details are not implemented yet*/
            MouseArea {
                anchors.fill: parent
                onClicked: {
                /*    appWindow.viewChanged(gameid);
                    pageStack.push(detailsPage);*/
                }
            }


            // Label containing the total score
            Text {
                id: totalScore

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: periodsScore.top
                }

                font {
                    family: "Nokia Pure Text"
                    pointSize: 18
                    weight: Font.Bold
                }
                text: totalscore
            }

            // Label containing the per-period score
            Text {
                id: periodsScore

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: homeLogo.verticalCenter
                }

                font {
                    family: "Nokia Pure Light"
                    pointSize: 18
                }

                text: "(" + periodsscore + ")"
            }

            // Label indicating the status of the game
            Text {
                id: gameStatus

                font {
                    family: "Nokia Pure Light"
                    pointSize: 14
                }

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: periodsScore.bottom
                }

                text: gamestatus
            }

            // Hometeam logo: to the left
            Image {
                id: homeLogo

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 10
                }

                height: 96
                width: 96
                fillMode: Image.PreserveAspectFit
                source: "icons/" + hometeamId + ".png"
            }

            // Awayteam logo: to the right
            Image {
                id: awayLogo

                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 10
                }

                height: 96
                width: 96
                fillMode: Image.PreserveAspectFit
                source: "icons/" + awayteamId + ".png"
            }

            // Separator between the entries
            Image {
                id: gameSeparator
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                source: "image://theme/meegotouch-groupheader-background"
            }
        }
    }

    ListView {
        id: gameList
        width: parent.width
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        model: listData
        delegate: gameDelegate
    }
}
