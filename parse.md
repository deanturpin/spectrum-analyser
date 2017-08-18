```bash
while read line; do [[ $line =~ ([^\ ]+)\ +([^\ ]+) ]] && echo \{${BASH_REMATCH[2]}, \"${BASH_REMATCH[1]}\"\},; done < notes.md | sed 's/\/[^"]*//'
```
