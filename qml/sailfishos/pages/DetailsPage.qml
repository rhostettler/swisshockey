import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    SilicaListView {
        id: gameEvents
        width: parent.width
        anchors.fill: parent
        model: gameEventsData

        header: Item {
            id: gameSummary
            width: parent.width
            height: Theme.itemSizeExtraLarge
            //contentHeight: Theme.itemSizeExtraLarge
            //z: 1

            /*Rectangle {
                anchors.fill: parent
                color: "white"
            }*/

            // Label containing the total score
            Label {
                id: detailsTotalScore
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: detailsPeriodsScore.top
                }
                font {
                    family: Theme.fontFamilyHeading
                    pixelSize: Theme.fontSizeMedium
                }
                color: Theme.highlightColor

                // Automagically updated because of Q_PROPERTY has NOTIFY set
                text: gameDetailsData.totalScore
            }

            // Label containing the per-period score
            Label {
                id: detailsPeriodsScore
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: detailsHomeLogo.verticalCenter
                }
                font {
                    pixelSize: Theme.fontSizeMedium
                }
                color: Theme.secondaryColor

                // Automagically updated because of Q_PROPERTY has NOTIFY set
                text: "(" + gameDetailsData.periodsScore + ")"
            }

            // Label indicating the status of the game
    /*        Label {
                id: gameStatus
                font {
                    pixelSize: Theme.fontSizeMedium
                }
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: periodsScore.bottom
                }
                color: Theme.secondaryColor
                text: gamestatus
            }*/

            // Hometeam logo: to the left
            Image {
                id: detailsHomeLogo

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                }

                height: 96
                width: 96
                fillMode: Image.PreserveAspectFit
                source: "../../icons/" + gameDetailsData.hometeamId + ".png"
            }

            // Awayteam logo: to the right
            Image {
                id: detailsAwayLogo

                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                height: 96
                width: 96
                fillMode: Image.PreserveAspectFit
                source: "../../icons/" + gameDetailsData.awayteamId + ".png"
            }
        }

        delegate: Item{
            width: gameEvents.width
            height: Theme.itemSizeLarge

            // The time to the very left
            Label {
                id: time
                width: 96
                //x: Theme.horizontalPageMargin
                anchors {
                    left: parent.left
                    bottom: parent.verticalCenter
                    leftMargin: Theme.horizontalPageMargin
                }
                font {
                    family: Theme.fontFamilyHeading
                    pixelSize: Theme.fontSizeMedium
                }
                color: Theme.highlightColor
                text: eventtime
            }

            // Label containing the player
            Label {
                id: player
                anchors {
                    left: time.right
                    bottom: parent.verticalCenter
                }
                font {
                    family: Theme.fontFamilyHeading
                    pixelSize: Theme.fontSizeMedium
                }
                color: Theme.highlightColor
                text: eventplayer
            }

            // Label containing the assist or penalty type
            Label {
                id: additionalInfo
                anchors {
                    left: time.right
                    top: parent.verticalCenter
                }
                font {
                    pixelSize: Theme.fontSizeMedium
                }
                color: Theme.secondaryColor
                text: eventinfo
            }

            // Label containing the score or the penalty
            Label {
                id: eventText
                horizontalAlignment: Text.Right
                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    bottom: parent.verticalCenter
                    //verticalCenter: parent.verticalCenter
                }
                font {
                    family: Theme.fontFamilyHeading
                    pixelSize: Theme.fontSizeMedium
                }
                color: Theme.highlightColor
                text: eventtext
            }

            // Additional text under the score/penalty
            Label {
                id: eventSubtext
                horizontalAlignment: Text.Center
                anchors {
                    horizontalCenter: eventText.horizontalCenter
                    rightMargin: Theme.horizontalPageMargin
                    top: parent.verticalCenter
                }

                font {
                    pixelSize: Theme.fontSizeMedium
                }
                color: Theme.secondaryColor
                text: eventsubtext
            }
        }

        VerticalScrollDecorator {}
    }
}
