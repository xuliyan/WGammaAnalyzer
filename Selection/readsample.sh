name1="/isilon/hadoop/store/user/xuyan/SinglePhoton/Wgamma94XSinglePhoton_Jun19_2017B/190619_110227/0000/"
name2=" 0 NONE"
name0="$ SinglePhoton2017B"
files=$(ls /isilon/hadoop/store/user/xuyan/SinglePhoton/Wgamma94XSinglePhoton_Jun19_2017B/190619_110227/0000)
echo ${name0} >> SinglePhoton_2017B_Jun19.conf
for line in $files
do
echo $line
name3=${name1}${line}${name2}
echo ${name3} >> SinglePhoton_2017B_Jun19.conf
done