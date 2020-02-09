def remove_duplicates(infile):
    storehouse = set()
    with open("outfile.txt","w+", encoding="Latin-1") as out:
        for line in open(infile):
            if line not in storehouse:
                out.write(line)
                storehouse.add(line)
                
lines = open('rockyou.txt', 'r', encoding="Latin-1").readlines()



result = [s for s in lines if not (s.strip("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM\." +"\n"  + "1234567890") or len(s) != 9)]
file2 = open("psw.txt","a", encoding="Latin-1")
lines_set = set(result)
count = 0
for s in lines_set:
    file2.write(s)
    count += 1

print(count)
remove_duplicates("psw.txt")


