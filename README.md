# fiveTargetGame
"Biathlon" like game, with 5-target embedded firmware, gun firmware and Android BT application

if this error appears:
```
avrdude: Version 6.3, compiled on Sep 12 2016 at 15:21:49
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "/home/jenkins/workspace/avrdude/label/Ubuntu12.04x64/objdir/etc/avrdude.conf"
avrdude: can't open config file "/home/jenkins/workspace/avrdude/label/Ubuntu12.04x64/objdir/etc/avrdude.conf": No such file or directory
avrdude: error reading system wide configuration file "/home/jenkins/workspace/avrdude/label/Ubuntu12.04x64/objdir/etc/avrdude.conf"
```
it is a bug in Avrdude. Open `/.platformio/packages/tool-avrdude/avrdude` and change:
```
exec -a "$0" "`dirname "$0"`/avrdude_bin" "$@"
```
by
```
exec -a "$0" "`dirname "$0"`/avrdude_bin" "-C${HOME}/.platformio/packages/tool-avrdude/avrdude.conf" "$@"
```