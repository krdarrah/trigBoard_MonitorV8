void initMonitorUDP() {

  if (udp.listen(config.udpPort)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {// all data comes in here
      for (int i = 0; i < packet.length(); i++) {
        newPacket[i] = char(packet.data()[i]);
        newPacket[i + 1] = NULL;
      }
      if (strcmp(newPacket, oldPacketReceived) != 0) {//don't match, so must be new
        Serial.println("something new");
        
        strcpy(oldPacketReceived, newPacket);
        receivedPackets = 0;
        firstPacket = true;

        //        //Good debug info
        //        Serial.print("UDP Packet Type: ");
        //        Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
        //        Serial.print(", From: ");
        //        Serial.print(packet.remoteIP());
        //        Serial.print(":");
        //        Serial.print(packet.remotePort());
        //        Serial.print(", To: ");
        //        Serial.print(packet.localIP());
        //        Serial.print(":");
        //        Serial.print(packet.localPort());
        //        Serial.print(", Data: ");

        Serial.print("Playing Track:");
        int trackToPlay = atoi(newPacket);
        Serial.println(trackToPlay);
        //here's where we can play the track
        donePlaying = false;
        digitalWrite(speakerPwrPin, LOW);
        sendCommand(CMD_PLAY_FOLDER_FILE, 1 , trackToPlay);
        delay(100);
        if (mp3.available())
        {
          Serial.println(decodeMP3Answer());
          mp3.flush();
        }
        delay(500);
        unsigned long mp3StartTime = millis();
        while (millis() - mp3StartTime < 5000) {
          sendCommand(CMD_QUERY_STATUS);
          delay(20);
          if (mp3.available())
          {
            //decodeMP3Answer();
            Serial.println(decodeMP3Answer());
            mp3.flush();
            if (donePlaying) {
              Serial.println("Done playing");
              break;
            }
          }
        }
        //udp.flush();
        delay(100);
        digitalWrite(speakerPwrPin, HIGH);
      } else {
        receivedPackets++;
        Serial.println(receivedPackets);
      }

    });
  }




}
