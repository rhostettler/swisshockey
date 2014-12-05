import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    tools: mainToolbar

    // Shows the game summary
    Item {
        id: gameSummary
        width: parent.width
        height: 128

        // Label containing the total score
        Text {
            id: detailsTotalScore

            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: detailsPeriodsScore.top
            }

            font {
                family: "Nokia Pure Text"
                pointSize: 18
                weight: Font.Bold
            }
            text: detailstotalscore
        }

        // Label containing the per-period score
        Text {
            id: detailsPeriodsScore

            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: detailsHomeLogo.verticalCenter
            }

            font {
                family: "Nokia Pure Light"
                pointSize: 18
            }

            text: "(" + detailsperiodsscore + ")"
        }

        // Label indicating the status of the game
/*        Text {
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
        }*/

        // Hometeam logo: to the left
        Image {
            id: detailsHomeLogo

            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 10
            }

            height: 96
            width: 96
            fillMode: Image.PreserveAspectFit
            source: "icons/" + detailshometeam + ".gif"
        }

        // Awayteam logo: to the right
        Image {
            id: detailsAwayLogo

            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 10
            }

            height: 96
            width: 96
            fillMode: Image.PreserveAspectFit
            source: "icons/" + detailsawayteam + ".gif"
        }

        // Separator between the entries
        Image {
            id: detailsGameSeparator
            anchors {
                left: parent.left
                right: parent.right
                top: parent.bottom
            }

            source: "image://theme/meegotouch-groupheader-background"
        }
    }

    /*BusyIndicator {
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        platformStyle: BusyIndicatorStyle { size: "large" }
        running: gameList.count == 0
        visible: gameList.count == 0
    }*/

    Component {
        id: eventsDelegate

        Item{
            width: gameEvents.width
            height: 96

            // The time to the very left
            Text {
                id: time

                width: 96
                //height: parent.height
                //verticalAlignment: Text.AlignVCenter
                //horizontalAlignment: Text.Right

                anchors {
                    left: parent.left
                    bottom: parent.verticalCenter
                    leftMargin: 10
                    horizontalCenter: parent.Center
                }

                font {
                    family: "Nokia Pure Text"
                    pointSize: 18
                    weight: Font.Bold
                }

                text: eventtime
            }

            // Label containing the player
            Text {
                id: player

                anchors {
                    //horizontalCenter: parent.left
                    left: time.right
                    bottom: parent.verticalCenter
                }

                font {
                    family: "Nokia Pure Text"
                    pointSize: 18
                    weight: Font.Bold
                }

                text: eventplayer
            }

            // Label containing the assist or penalty type
            Text {
                id: additionalInfo

                anchors {
                    left: time.right
                    top: parent.verticalCenter
                }

                font {
                    family: "Nokia Pure Light"
                    pointSize: 18
                }

                text: eventinfo
            }

            // Label containing the score or the penalty
            Text {
                id: eventText
                width: 96
                //height: parent.height
                //verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.Right

                anchors {
                    right: parent.right
                    rightMargin: 10
                    bottom: parent.verticalCenter
                    //verticalCenter: parent.verticalCenter
                }

                font {
                    family: "Nokia Pure Light"
                    pointSize: 18
                    weight: Font.Bold
                }

                text: eventtext
            }

            // Separator between the entries
            Image {
                id: eventSeparator
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.bottom
                }

                source: "image://theme/meegotouch-groupheader-background"
            }
        }
    }

    ListView {
        id: gameEvents
        width: parent.width
        anchors {
            top: gameSummary.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        model: gameEventsData
        delegate: eventsDelegate
    }
}
