import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1 // for InfoBanner

PageStackWindow {
    id: appWindow
    initialPage: overviewPage

    // Emitted when one of the games is selected for details view
    signal viewChanged(string gameId);
    signal updateTriggered();
    signal leagueChanged(string league);

    // Create an info banner with no icon
    InfoBanner {
        topMargin: 40
        id: infoBanner
        text: ""
    }

    function showInfo(message) {
        infoBanner.text = message;
        infoBanner.show();
    }

    // Define the pages
    GamesOverviewPage {
        id: overviewPage
        objectName: "overviewPage"
    }

    GameDetailsPage {
        id: detailsPage
    }

    ToolBarLayout {
        id: mainToolbar
        visible: true

        ToolIcon {
            iconId:  (pageStack.depth > 1) ? "toolbar-back" : "toolbar-back-dimmed"
            visible: true

            onClicked: {
                pageStack.pop();
            }
        }

        // List icon: Will be used in the details view and shows the game events
/*        ToolIcon {
            iconId: "toolbar-list"
            visible: (pageStack.depth !== 1)
        }*/

        // Silhouette icon: Will be used to switch to the roster (future thing)
/*        ToolIcon {
            iconId: "toolbar-contact"
            visible: (pageStack.depth !== 1)
        }*/

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            //visible: (pageStack.depth === 1)
            visible: true
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (mainMenu.status === DialogStatus.Closed) ? mainMenu.open() : mainMenu.close()
        }
    }

    Menu {
        id: mainMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("About")
                onClicked: aboutDialog.open();
            }
        }
    }

    QueryDialog {
        id: aboutDialog
        titleText: qsTr("About")
        message: qsTr("A simple app providing live results from the Swiss national ice hockey leagues, National League A and B."
                      + "<br><br>"
                      + "Version 0.2.0"
                      + "<br><br>"
                      + "<a href=\"https://github.com/rolandh83/nl-live-scores\">https://github.com/rolandh83/nl-live-scores</a>"
                      )
        acceptButtonText: qsTr("Close")
    }
}
