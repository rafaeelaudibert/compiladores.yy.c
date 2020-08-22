cd $1
make clean
tar cvzf $1.tgz .

# This way, it can be downloaded at https://inf.ufrgs.br/~rbaudibert/$1.tgz
scp $1.tgz rbaudibert@html.inf.ufrgs.br:public_html/