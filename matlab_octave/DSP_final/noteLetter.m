function Letter = noteLetter(noteNum);

if noteNum == 0
    Letter = "A440";
elseif noteNum == 1
    Letter = "A#";
elseif noteNum == 2
    Letter = "B";
elseif noteNum == 3
    Letter = "C";
elseif noteNum == 4
    Letter = "C#";
elseif noteNum == 5
    Letter = "D";
elseif noteNum == 6
    Letter = "D#";
elseif noteNum == 7
    Letter = "E";
elseif noteNum == 8
    Letter = "F";
elseif noteNum == 9
    Letter = "F#";
elseif noteNum == 10
    Letter = "G";
elseif noteNum == 11
    Letter = "G#";
elseif noteNum == 12
    Letter = "A880";
else
    Letter = "ERROR";
endif

