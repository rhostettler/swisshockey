nl-live-scores
==============
NLLiveScores is an app for the Nokia N9 providing live scores from the Swiss
national ice hockey leagues, National League A and B.

Roadmap
-------
*Version 0.2.0*
[ ] Notification callback action
[ ] Full support for NL B (Notifications in cp missing)
[X] Proper league filtering

*Version 1.0.0*
[ ] Game details support
[ ] Code cleanup

Known Issues
------------
General:
* Implement a calendar-view
* The cpapplet is kind of a mess

Notifications:
* Instead of completely disabling notifications when in foreground, I should 
  consider just beeping without homescreen notification.
* Notifications are not cleared on application shutdown
* Use notification groups (?).
* Tapping the notification doesn't open the app.

Game details view:
* Overtime- and shootout events are sorted at the bottom.
* One should be able to distinguish which event belongs to which team 
  more easily (Consider implementing event icons and logos).
* Game roaster is not implemented.
* Game summary at the top doesn't update score.
* Times < 1min are not parsed correctly

