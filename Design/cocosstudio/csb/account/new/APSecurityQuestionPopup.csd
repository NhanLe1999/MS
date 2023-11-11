<GameFile>
  <PropertyGroup Name="APSecurityQuestionPopup" Type="Layer" ID="b3e1d61e-6541-43d0-a3ce-b835615d06e8" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APSecurityQuestionPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-394346387" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="102.0000" RightMargin="102.0000" TopMargin="54.0000" BottomMargin="54.0000" Scale9Enable="True" LeftEage="17" RightEage="17" TopEage="13" BottomEage="13" Scale9OriginX="17" Scale9OriginY="13" Scale9Width="60" Scale9Height="56" ctype="ImageViewObjectData">
            <Size X="820.0000" Y="660.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="72574305" CallBackType="Click" CallBackName="onCancel" Tag="2178" IconVisible="False" LeftMargin="763.6938" RightMargin="-3.6938" TopMargin="-3.1072" BottomMargin="603.1072" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-1924488657" Tag="3423" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="35.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="30.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5833" Y="0.5833" />
                    <FileData Type="Normal" Path="account/exit.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="793.6938" Y="633.1072" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9679" Y="0.9593" />
                <PreSize X="0.0732" Y="0.0909" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1717790470" Tag="362" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="26.5000" RightMargin="26.5000" TopMargin="47.0000" BottomMargin="547.0000" FontSize="27" LabelText="Bạn vui lòng trả lời các câu hỏi sau để xác thực tài khoản.&#xA;" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="767.0000" Y="66.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="410.0000" Y="580.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8788" />
                <PreSize X="0.9354" Y="0.1000" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="quest_01" ActionTag="-149785760" Tag="1292" IconVisible="False" LeftMargin="50.0000" RightMargin="324.0000" TopMargin="134.0000" BottomMargin="491.0000" FontSize="29" LabelText="1.Tài khoản có mấy hồ sơ học ?" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="446.0000" Y="35.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="50.0000" Y="508.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="26" G="26" B="26" />
                <PrePosition X="0.0610" Y="0.7705" />
                <PreSize X="0.5439" Y="0.0530" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq1_1" ActionTag="-719363950" Tag="1291" IconVisible="False" LeftMargin="95.0000" RightMargin="699.0000" TopMargin="199.0000" BottomMargin="434.0000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as1_1" ActionTag="1779081004" CallBackType="Click" CallBackName="onAnsClicked" Tag="2454" IconVisible="False" LeftMargin="38.3306" RightMargin="-23.3306" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="1" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="11.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3306" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4743" Y="0.5880" />
                    <PreSize X="0.4231" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="95.0000" Y="447.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1159" Y="0.6780" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq1_2" ActionTag="523377236" Tag="2455" IconVisible="False" LeftMargin="268.0000" RightMargin="526.0000" TopMargin="199.0000" BottomMargin="434.0000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as1_2" ActionTag="111746391" CallBackType="Click" CallBackName="onAnsClicked" Tag="2456" IconVisible="False" LeftMargin="38.3306" RightMargin="-31.3306" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="19.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3306" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4743" Y="0.5880" />
                    <PreSize X="0.7308" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="268.0000" Y="447.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3268" Y="0.6780" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq1_3" ActionTag="558687465" Tag="2457" IconVisible="False" LeftMargin="454.0000" RightMargin="340.0000" TopMargin="199.0000" BottomMargin="434.0000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as1_3" ActionTag="-503871995" CallBackType="Click" CallBackName="onAnsClicked" Tag="2458" IconVisible="False" LeftMargin="38.3306" RightMargin="-30.3306" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="18.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3306" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4743" Y="0.5880" />
                    <PreSize X="0.6923" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="454.0000" Y="447.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5537" Y="0.6780" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq1_4" ActionTag="140856026" Tag="2459" IconVisible="False" LeftMargin="621.0000" RightMargin="173.0000" TopMargin="199.0000" BottomMargin="434.0000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as1_4" ActionTag="291024147" CallBackType="Click" CallBackName="onAnsClicked" Tag="2460" IconVisible="False" LeftMargin="38.3300" RightMargin="-34.3300" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="22.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3300" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4742" Y="0.5880" />
                    <PreSize X="0.8462" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="621.0000" Y="447.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7573" Y="0.6780" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="continute_button" ActionTag="-235109591" CallBackType="Click" CallBackName="onContinute" Tag="1303" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="277.0000" RightMargin="277.0000" TopMargin="551.5000" BottomMargin="39.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="266.0000" Y="69.0000" />
                <Children>
                  <AbstractNodeData Name="send_title" ActionTag="556828574" Tag="1304" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="79.5000" RightMargin="79.5000" TopMargin="19.5000" BottomMargin="19.5000" FontSize="25" LabelText="Tiếp tục" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="107.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="133.0000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4023" Y="0.4348" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="410.0000" Y="74.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1121" />
                <PreSize X="0.3244" Y="0.1045" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="quest_02" ActionTag="-472203972" Tag="2461" IconVisible="False" LeftMargin="50.0000" RightMargin="165.0000" TopMargin="272.0000" BottomMargin="353.0000" FontSize="29" LabelText="2. Đây có phải hồ sơ học của bạn không ?" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="605.0000" Y="35.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="50.0000" Y="370.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="26" G="26" B="26" />
                <PrePosition X="0.0610" Y="0.5614" />
                <PreSize X="0.7378" Y="0.0530" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq2_1" ActionTag="1066762323" Tag="2462" IconVisible="False" LeftMargin="95.0000" RightMargin="699.0000" TopMargin="351.0000" BottomMargin="282.0000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as2_1" ActionTag="-1445217880" CallBackType="Click" CallBackName="onAnsClicked" Tag="2463" IconVisible="False" LeftMargin="38.3306" RightMargin="-23.3306" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="1" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="11.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3306" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4743" Y="0.5880" />
                    <PreSize X="0.4231" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="95.0000" Y="295.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1159" Y="0.4477" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq2_2" ActionTag="466284195" Tag="2464" IconVisible="False" LeftMargin="511.0000" RightMargin="283.0000" TopMargin="351.5000" BottomMargin="281.5000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as2_2" ActionTag="1749774868" CallBackType="Click" CallBackName="onAnsClicked" Tag="2465" IconVisible="False" LeftMargin="38.3306" RightMargin="-31.3306" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="19.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3306" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4743" Y="0.5880" />
                    <PreSize X="0.7308" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="511.0000" Y="295.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6232" Y="0.4470" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq2_3" ActionTag="184324972" Tag="2466" IconVisible="False" LeftMargin="95.0000" RightMargin="699.0000" TopMargin="445.0000" BottomMargin="188.0000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as2_3" ActionTag="-1087020581" CallBackType="Click" CallBackName="onAnsClicked" Tag="2467" IconVisible="False" LeftMargin="38.3306" RightMargin="-30.3306" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="18.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3306" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4743" Y="0.5880" />
                    <PreSize X="0.6923" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="95.0000" Y="201.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1159" Y="0.3053" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="aswq2_4" ActionTag="1047320132" Tag="2468" IconVisible="False" LeftMargin="511.0000" RightMargin="283.0000" TopMargin="445.0000" BottomMargin="188.0000" TouchEnable="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="as2_4" ActionTag="-1499081380" CallBackType="Click" CallBackName="onAnsClicked" Tag="2469" IconVisible="False" LeftMargin="38.3300" RightMargin="-34.3300" TopMargin="-6.3748" BottomMargin="-1.6252" FontSize="29" LabelText="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="22.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="38.3300" Y="15.8748" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.4742" Y="0.5880" />
                    <PreSize X="0.8462" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="511.0000" Y="201.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6232" Y="0.3053" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8008" Y="0.8594" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>