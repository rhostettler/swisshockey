import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    tools: mainToolbar

    // A spinning busy indicator shown while the data is loading upon
    // application start
    BusyIndicator {
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        platformStyle: BusyIndicatorStyle { size: "large" }
        running: gameList.count == 0
        visible: gameList.count == 0
    }

    // The delegate that draws the different games
   Component {
        id: gameDelegate

        Item{
            width: gameList.width
            height: 128

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    appWindow.viewChanged(gameid)
                    pageStack.push(detailsPage);
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
        anchors.fill: parent

        model: listData
        delegate: gameDelegate
    }
}
