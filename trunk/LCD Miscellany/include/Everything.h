// Just one more effort to make LCDMisc Script.c
// require fewer changes on update. Note that
// removing unused files saves only a small
// amount of memory and no CPU time.

#requires <framework\Theme.c>
#requires <framework\framework.c>

#requires <Views\DualView.c>
#requires <Views\StatusView.c>
#requires <Views\WeatherView.c>
#requires <Views\TaskManager.c>
#requires <Views\MediaView.c>
#requires <Views\ClipboardView.c>
#requires <Views\CalculatorView.c>
#requires <Views\DownloadView.c>
#requires <Views\SABnzbdView.c>
#requires <Views\TextEditorView.c>

//#requires <Views\JabberView.c>
//#requires <Views\PidginView.c>
//#requires <Views\CommandView.c>
//#requires <Views\ChatView.c>

// Has constants in it, so needs #import
#import <Views\RSSView.c>


#requires <ChatServices\IRCChatService.c>


#requires <Multimedia\mpc.c>
#requires <Multimedia\vlc.c>
#requires <Multimedia\winamp.c>
#requires <Multimedia\itunes.c>
#requires <Multimedia\amip.c>

#requires <Status\QuadStatus.c>
#requires <Status\DefaultStatus.c>
#requires <Status\GameStatus.c>
#requires <Status\SpeedFanStatus.c>
#requires <Status\G19Status.c>

#requires <framework\Alert.c>

#requires <Modules\SysTrayIcons.c>
