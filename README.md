nl-live-scores
==============
NLLiveScores is an app for the Nokia N9 providing live scores from the Swiss
national ice hockey leagues, National League A and B.

Known Issues
------------
Issues marked as "OK" are in testing state.

General:
* [OK] Timeout of the server is not handled, both initially and on update
  requests.
* Make architecture more generic -> implement data sources that do all the 
  parsing while local data structures are used for storing the data. Also, 
  filtering should be done on a presentation level.
* Implement a calendar-view
* Adapt to the new API
* NLB is not implemented yet.
* UTF8-parsing of player names is not complete yet.
* Use pull-down gesture for manual update.
* The settings applet is kind of a mess

Notifications:
* Notifications are sent out even when the application is in the 
  foreground.
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


