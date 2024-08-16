alias pio="~/.platformio/penv/Scripts/platformio.exe"

# check if git bash on windows
if [ -n "$WINDIR" ]; then
    source ./venv/Scripts/activate
else
    source ./venv/bin/activate
fi