# PrintFile-Utility
This [utility]((https://ru.wikipedia.org/wiki/%D0%A3%D1%82%D0%B8%D0%BB%D0%B8%D1%82%D0%B0)) allows you to output the contents of a text file to a standard output stream. The program supports the following command line arguments (options):  
 - **-l, --lines=n** - *output only n (positive only) first (last) lines of the file (optional argument, by default all lines are output).*
 - **-t, --tail** - *output n last lines of the file (optional argument).*
 - **-d, --delimiter=c** - *character by which the end of the line is determined (optional, default is '\n'), parameter can be passed in single quotes, double quotes or no quotes at all.*

## **Notes:**  

 1) Delimiter parameter can be passed in single quotes, double quotes or no quotes at all.  
 2) You can specify a parameter for <u>lines</u> and <u>delimiter</u> only once.  
 3) The --tail (-t) option can be entered as many times as you like.
 4) Options and file name can be arranged relative to each other as desired.

## **Requirements:**
According to the requirements of lab-work, the use of the STL is prohibited.

## **Program start format:**

`PrintFile.exe [options] file_path`

## **Launch examples:**  

`PrintFile.exe -l 10 --delimiter='a' ./filename.txt` - *print the first 10 lines with delimiter character 'a'.*  
`PrintFile.exe -d \t ./filename.txt --lines=123` - *print the first 123 lines with a tab delimiter.*  
`PrintFile.exe ./filename.txt -t --lines=12345` - *print the last 12345 lines with the delimiter as a line break character.*  
`PrintFile.exe --tail --delimiter=">" ./filename.txt -l 21 -t` - *print the last 21 lines with the delimiter as '>' character.*  
`PrintFile.exe ./filename.txt` - *print the entire file.*  

## **Test file**  
### File.txt content:
```
1;2;3;4;
5;6;7;8;
9;10;11;
12
```  
Command line: `PrintFile.exe File.txt --lines=5 -d ';'`  
### Output:  
```
1;2;3;4;
5;
```  
Command line: `PrintFile.exe File.txt --lines=3 -d ';' --tail`  
### Output:  
```
10;11;
12
```