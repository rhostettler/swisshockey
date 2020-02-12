# Swiss Ice Hockey
A native Sailfish OS app that provides live scores from the Swiss men's national ice hockey leagues, National League and Swiss League.

Current features include:
* Live scores for the National and Swiss Leagues, and the Swiss Ice Hockey Cup.
* Basic goal notification (currently for a selected game only).
* Team rosters.


## TODO / Desirable Features
This is a list of desirable features, mainly for keeping track of some ideas. Since this is a spare-time project, these features might be implemented tomorrow, in one month, two years from now, or not at all and in no particular order. 

General:
* [ ] Add support for MySports League
* [ ] Add support for standings
* [ ] Add support for past / future games
* [ ] Preferences page with the option to select teams for notifications, etc.

Notifications:
* [ ] Callback actions
* [ ] Banner notification when in foreground
* [ ] Notification grouping
* [ ] Event details (Player name, time, etc.)

Data handling:
* [ ] Player stats parsing
* [ ] Game stats parsing
* [ ] Only use summaries to find all the games, then use individual game detail calls for each game

Code cleanup:
* [ ] Simplify `GameList`
* [ ] Use Q_FUNC_INFO everywhere in `Logger`. Add more logging where needed.
* [ ] Get rid of all compiler warnings
* [ ] Get rid of all QML warnings

Backend:
* [ ] Implement focus and view change signals (for notifications mainly)
* [ ] Check if JSONDecoder can be simplified for Qt 5

User interface:
* [ ] Rework cover with an overview instead of the selected game
* [ ] Add update cover action
* [ ] Game start times should be shown according to locale
* [ ] Move to ViewType-struct for view-transitions (see 
    https://stackoverflow.com/questions/20089196/how-to-access-c-enum-from-qml#20108038)
* [ ] Check if we can use the same "Delegate"/"Item" to draw the game info in the game list page as well as in the header in the details page
* [ ] Make the UI scale to tablets


## Known Issues
* [ ] Updates are sometimes not triggered automatically.
* [ ] In details view, score does not update at the same time as the details.


## License
This software is free software and licensed under the GNU GPL version 3. The 
teams' logos included are trademarks of the respective team and used on a 
fair-use basis.

