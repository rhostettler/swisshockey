import QtQuick 1.1
import com.nokia.meego 1.0

Page {
//    orientationLock: Page.lockInPortrait
    //tools: commonTools

    Component {
        id: gameDelegate

        Item{
            property variant statuses: [
                "Not Started",    // 0
                "First Period",
                "End of First",
                "Second Period",
                "End of Second",
                "Third Period",   // 5
                "End of Third",
                "Unknown (7)",
                "Unknown (8)",
                "Unknown (9)",
                "Unknown (10)",
                "Unknown (11)",
                "Unknown (12)"
            ]

            width: gameList.width
            height: 128

            // Label containing the total score
            Text {
                id: totalScore

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: homeLogo.verticalCenter
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
                    horizontalCenter: totalScore.horizontalCenter
                    top: homeLogo.verticalCenter
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
                }

                anchors {
                    horizontalCenter: periodsScore.horizontalCenter
                    top: periodsScore.bottom
                }

                text: statuses[gamestatus]
            }

            // Hometeam logo: to the left
            Image {
                id: homeLogo

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 5
                }

                height: 96
                width: 96
                fillMode: Image.PreserveAspectFit
                source: "icons/" + hometeam + ".gif"
            }

            // Awayteam logo: to the right
            Image {
                id: awayLogo

                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 5
                }

                height: 96
                width: 96
                fillMode: Image.PreserveAspectFit
                source: "icons/" + awayteam + ".gif"
            }

            // Separator between the entries
            Image {
                id: gameSeparator
                anchors {
                    left: parent.left
                    right: parent.right
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
