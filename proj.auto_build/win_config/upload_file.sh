pathZipFile=$(find '*.zip')

responceZIP=$(curl --location --request POST 'https://media.monkeyuni.net/api/upload' \
--header 'Referer: https://crm.monkey.edu.vn/' \
--header 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36' \
--header 'token:a813ec766197294184a938c331b08e7e' \
--form 'file=@"$pathZipFile"' \
--form 'folder_path="upload/app/auto_build/monkey_stories"' \
--form 'only_url=1')


curl -s -X POST https://api.telegram.org/bot899610366:AAHp5YC42hde5_ews56KeseKUp_untz3kNs/sendMessage -d chat_id=-646869145 -d text="MONKEY STORIES - BUILD WINDOWS DONE:"
curl -s -X POST https://api.telegram.org/bot899610366:AAHp5YC42hde5_ews56KeseKUp_untz3kNs/sendMessage -d chat_id=-646869145 -d text="APK Debug link: $responceZIP"
