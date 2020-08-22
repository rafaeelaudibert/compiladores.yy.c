FOLDER=$1
SCP_PASSWORD=$2

cd ${FOLDER}

echo "Taring to ${FOLDER}.tgz"
tar cvzf ${FOLDER}.tgz .

# This way, it can be downloaded at https://inf.ufrgs.br/~rbaudibert/$1.tgz
echo "sending through SCP to rbaudibert@html.inf.ufrgs.br..."
sshpass -p ${SCP_PASSWORD} scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null ${FOLDER}.tgz rbaudibert@html.inf.ufrgs.br:public_html/