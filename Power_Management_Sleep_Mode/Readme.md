In this example, I used the STM32 NUCLEO-F030R8 (MB1136 C-04) development board. I run the project on sleep mode because of power efficiency. I used USART2 and the PA1 pin for external interrupts to wake up the system from sleep mode by sending a character or a 3.3V signal. I set PA1 pin at pull-down mode for stability. I used callback functions just for set the wakeup_source variable.


<img width="1418" height="348" alt="guc_yonetimi_1" src="https://github.com/user-attachments/assets/9f752b50-c30b-40ff-9125-447d309860bc" />

<img width="1477" height="299" alt="guc_yonetimi_2" src="https://github.com/user-attachments/assets/9d2afab4-14e8-42c1-8079-7c7a6085bd77" />

<img width="1242" height="637" alt="guc_yonetimi_3" src="https://github.com/user-attachments/assets/9ca1f0e1-1d35-4ecf-a8b3-3d34ac46187e" />

<img width="1482" height="503" alt="guc_yonetimi_4" src="https://github.com/user-attachments/assets/2abd4fac-7a84-4fc2-8e92-b3991dd69fe7" />

<img width="1412" height="551" alt="guc_yonetimi_5" src="https://github.com/user-attachments/assets/435a63db-eec7-4138-bbfc-8874466beddf" />

<img width="1282" height="728" alt="guc_yonetimi_6" src="https://github.com/user-attachments/assets/1e3f6046-ba92-4bb1-bcea-fd60891865b0" />

<img width="1341" height="1066" alt="guc_yonetimi_7" src="https://github.com/user-attachments/assets/826b218b-f5f6-4105-838e-e1145d4adbb7" />

<img width="1341" height="1068" alt="guc_yonetimi_8" src="https://github.com/user-attachments/assets/3c3fe077-39ba-4478-9eb6-81e6c2cea00c" />

<img width="1342" height="1068" alt="guc_yonetimi_9" src="https://github.com/user-attachments/assets/9bbad612-c82e-413c-b489-2b732ec153be" />

<img width="1340" height="1066" alt="guc_yonetimi_10" src="https://github.com/user-attachments/assets/d2320b5b-c60f-4fed-9c3d-e2f863514a99" />

<img width="1343" height="1067" alt="guc_yonetimi_11" src="https://github.com/user-attachments/assets/0c9622e3-4be4-4ecb-854b-78bda6e1354d" />

<img width="1342" height="1067" alt="guc_yonetimi_12" src="https://github.com/user-attachments/assets/4dc064ac-2073-4506-b82f-fca9e5346472" />

<img width="1339" height="1068" alt="guc_yonetimi_13" src="https://github.com/user-attachments/assets/3f0e7440-1101-4e6d-8946-b14cbd180683" />

Citation: https://xbowtie.com/egitmen/arifmandal

<img width="1250" height="1065" alt="pwr_sleep_1" src="https://github.com/user-attachments/assets/108809e2-8012-476e-897e-61d835b046e0" />

<img width="1250" height="1067" alt="pwr_sleep_2" src="https://github.com/user-attachments/assets/7af1aadf-a86a-4f04-9570-b66411149a72" />

<img width="1247" height="1067" alt="pwr_sleep_3" src="https://github.com/user-attachments/assets/bc23aba7-b4a9-438f-ba0d-c61ec3048900" />

<img width="1249" height="1067" alt="pwr_sleep_4" src="https://github.com/user-attachments/assets/f8ce4a0c-272e-45f8-b7ee-e55d07f31e8c" />

<img width="1251" height="1069" alt="pwr_sleep_5" src="https://github.com/user-attachments/assets/1c8a71b3-ac4e-43d3-9ebb-8472032d6754" />

<img width="1251" height="1069" alt="pwr_sleep_6" src="https://github.com/user-attachments/assets/22bf309e-4972-4185-984b-45c449e0de9f" />

<img width="1250" height="1068" alt="pwr_sleep_7" src="https://github.com/user-attachments/assets/e9cac879-7c94-4965-9821-fb179cb89337" />

<img width="1251" height="1066" alt="pwr_sleep_8" src="https://github.com/user-attachments/assets/32936aaf-2a87-46b5-af78-ff38543ba385" />

<img width="1249" height="1069" alt="pwr_sleep_9" src="https://github.com/user-attachments/assets/b5771dfa-792a-401e-a3b5-58d989d0ab7e" />

<img width="1250" height="1067" alt="pwr_sleep_10" src="https://github.com/user-attachments/assets/715c6a22-7433-4b1b-b3a6-57ec3e3bf1ce" />

<img width="1249" height="1065" alt="pwr_sleep_11" src="https://github.com/user-attachments/assets/79106044-53df-4a49-a3ef-d1ae5a0708d8" />

<img width="1251" height="1067" alt="pwr_sleep_12" src="https://github.com/user-attachments/assets/c54e9d5e-7c25-40a3-8e9c-240d0d393a14" />

<img width="1251" height="1068" alt="pwr_sleep_13" src="https://github.com/user-attachments/assets/3cb5239f-b79b-498e-ae9d-0fa306ed6b73" />

<img width="1249" height="1069" alt="pwr_sleep_14" src="https://github.com/user-attachments/assets/1ddde541-7b93-431c-bf3f-9414c04e1aee" />

<img width="1250" height="1067" alt="pwr_sleep_15" src="https://github.com/user-attachments/assets/b6cb11c3-b550-472a-9f89-81d8e184c849" />

<img width="1251" height="1066" alt="pwr_sleep_16" src="https://github.com/user-attachments/assets/1a7c5d96-7a02-42bd-a281-6a6c1407587b" />

<img width="1251" height="1068" alt="pwr_sleep_17" src="https://github.com/user-attachments/assets/2ca003c8-8165-45bc-84bc-0762ff9dfeab" />

<img width="1249" height="1067" alt="pwr_sleep_18" src="https://github.com/user-attachments/assets/c3fa7ce5-718d-4940-9ead-fa0001fcec1a" />

<img width="1079" height="897" alt="pwr_sleep_19" src="https://github.com/user-attachments/assets/1dac3739-9aa8-4e9b-827d-c4c1e967550a" />

<img width="1080" height="897" alt="pwr_sleep_20" src="https://github.com/user-attachments/assets/99c83877-6821-4eaa-8f52-b16d12f46584" />

<img width="1075" height="897" alt="pwr_sleep_21" src="https://github.com/user-attachments/assets/85c33f24-a932-4303-b7fe-54a751f0b063" />

<img width="1078" height="899" alt="pwr_sleep_22" src="https://github.com/user-attachments/assets/f0144659-7cea-46b6-a678-bcc228cc5552" />

<img width="1078" height="897" alt="pwr_sleep_23" src="https://github.com/user-attachments/assets/881c7f97-a32d-43a9-96ef-5d429c511714" />

Citation: https://controllerstech.com/low-power-modes-in-stm32/



https://github.com/user-attachments/assets/286337d0-fb52-44f9-bda4-0178f86490c7
