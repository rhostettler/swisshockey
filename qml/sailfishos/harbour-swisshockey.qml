import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    id: appWindow
    initialPage: overviewPage
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    // Emitted when one of the games is selected for details view
    signal viewChanged(string gameId);
    signal updateTriggered();
    signal leagueChanged(string league);

    OverviewPage {
        id: overviewPage
        objectName: "overviewPage"
    }

    DetailsPage {
        id: detailsPage
    }
}
