import string

myList = []
filepath = 'rockyou.txt'
with open(filepath, encoding="Latin-1") as fp:
   line = fp.readline()
   cnt = 1
   while line:
       myList.append(line)
       line = fp.readline()
       cnt += 1

result = [s for s in myList if not (s.strip("qwertyuiopasdfghjklzxcvbQWERTYUIOPASDFGHJKLZXCVB" +"\n"  + "1234567890") or len(s) != 9)]
file2 = open("psw.txt","a", encoding="Latin-1")
for s in result:
    file2.write(s)



