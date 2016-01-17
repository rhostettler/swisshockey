import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1 // for InfoBanner

PageStackWindow {
    id: appWindow
    initialPage: overviewPage

    // Emitted when one of the games is selected for details view
    signal viewChanged(string gameId)

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
    }

    GameDetailsPage {
        id: detailsPage
    }

    ToolBarLayout {
        id: mainToolbar
        visible: true

        ToolIcon {
            iconId: "toolbar-back"
            visible: (pageStack.depth !== 1)

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
            visible: false
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (mainMenu.status === DialogStatus.Closed) ? mainMenu.open() : mainMenu.close()
        }
    }

    Menu {
        id: mainMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "NLA"
            }

            MenuItem {
                text: "NLB"
            }

/*            MenuItem {
                text: qsTr("About")
            }*/
        }
    }
}
