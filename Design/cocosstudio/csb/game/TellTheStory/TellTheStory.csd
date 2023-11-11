<GameFile>
  <PropertyGroup Name="TellTheStory" Type="Layer" ID="451e6cbc-6e63-4039-a427-64543b261290" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="TellTheStory" Tag="42" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg_1" ActionTag="143973907" Tag="43" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-338.0000" RightMargin="-338.0000" ctype="SpriteObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.6602" Y="1.0000" />
            <FileData Type="Normal" Path="games/tellTheStory/bg.jpg" Plist="" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="root_layout" ActionTag="-238346350" Tag="44" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="113" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="Sprite_3" ActionTag="-867432714" Tag="46" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="66.0000" RightMargin="66.0000" TopMargin="91.5000" BottomMargin="91.5000" ctype="SpriteObjectData">
                <Size X="892.0000" Y="585.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8711" Y="0.7617" />
                <FileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="speaker" ActionTag="-942658221" CallBackType="Click" CallBackName="onClickSpeaker" Tag="134" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="54.9000" RightMargin="874.1000" TopMargin="40.8200" BottomMargin="632.1800" TouchEnable="True" FontSize="14" ButtonText="Button" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="65" Scale9Height="73" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="95.0000" Y="95.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-1392494709" CallBackType="Click" Tag="135" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="14.0000" RightMargin="14.0000" TopMargin="15.5000" BottomMargin="15.5000" LeftEage="22" RightEage="22" TopEage="21" BottomEage="21" Scale9OriginX="22" Scale9OriginY="21" Scale9Width="23" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="67.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.5000" Y="47.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7053" Y="0.6737" />
                    <FileData Type="Normal" Path="games/tellTheStory/loa.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="102.4000" Y="679.6800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1000" Y="0.8850" />
                <PreSize X="0.0928" Y="0.1237" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/tellTheStory/icon.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Sprite_2" ActionTag="2088533566" Tag="45" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="201.5000" RightMargin="201.5000" TopMargin="2.0000" BottomMargin="578.0000" ctype="SpriteObjectData">
                <Size X="621.0000" Y="188.0000" />
                <Children>
                  <AbstractNodeData Name="Text_6" ActionTag="1774778086" Tag="71" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="99.9475" RightMargin="103.0525" TopMargin="43.5792" BottomMargin="40.4208" FontSize="36" LabelText="Hãy kể lại câu chuyện &#xA;theo hình gợi ý" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineSize="4" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="418.0000" Y="104.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="308.9475" Y="92.4208" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="138" G="51" B="47" />
                    <PrePosition X="0.4975" Y="0.4916" />
                    <PreSize X="0.6731" Y="0.5532" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="255" B="255" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon" ActionTag="-893224625" Tag="49" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="453.9582" RightMargin="14.0418" TopMargin="72.6968" BottomMargin="-37.6968" LeftEage="50" RightEage="50" TopEage="50" BottomEage="50" Scale9OriginX="50" Scale9OriginY="50" Scale9Width="53" Scale9Height="53" ctype="ImageViewObjectData">
                    <Size X="153.0000" Y="153.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="530.4582" Y="38.8032" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8542" Y="0.2064" />
                    <PreSize X="0.2464" Y="0.8138" />
                    <FileData Type="Normal" Path="images/Layer-49.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="672.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.8750" />
                <PreSize X="0.6064" Y="0.2448" />
                <FileData Type="Normal" Path="games/tellTheStory/Layer-50-copy.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="img_panel" ActionTag="1040252233" Tag="74" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="203.5200" BottomMargin="142.0800" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="422.4000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="353.2800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4600" />
                <PreSize X="1.0000" Y="0.5500" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_share" ActionTag="-556340590" CallBackType="Click" CallBackName="onShare" Tag="49" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="516.3800" RightMargin="270.6200" TopMargin="649.2200" BottomMargin="11.7800" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="207" Scale9Height="85" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="237.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="Image_7" ActionTag="-1834178343" Tag="65" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.4000" RightMargin="154.6000" TopMargin="16.5000" BottomMargin="16.5000" LeftEage="18" RightEage="18" TopEage="25" BottomEage="25" Scale9OriginX="18" Scale9OriginY="25" Scale9Width="34" Scale9Height="24" ctype="ImageViewObjectData">
                    <Size X="70.0000" Y="74.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.4000" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.5000" />
                    <PreSize X="0.2954" Y="0.6916" />
                    <FileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1" ActionTag="-476752437" Tag="66" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="96.7121" RightMargin="25.2879" TopMargin="30.4164" BottomMargin="33.5836" FontSize="30" LabelText="Chia sẻ" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="115.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="211.7121" Y="55.0836" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8933" Y="0.5148" />
                    <PreSize X="0.4852" Y="0.4019" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="138" G="51" B="47" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="634.8800" Y="65.2800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6200" Y="0.0850" />
                <PreSize X="0.2314" Y="0.1393" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_stop_play" ActionTag="-695837126" CallBackType="Click" CallBackName="onStopPlay" Tag="133" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="260.3800" RightMargin="526.6200" TopMargin="650.9864" BottomMargin="10.0136" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="207" Scale9Height="85" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="237.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="loa" ActionTag="1102980792" Tag="134" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="14.9000" RightMargin="157.1000" TopMargin="22.0000" BottomMargin="22.0000" LeftEage="18" RightEage="18" TopEage="25" BottomEage="25" Scale9OriginX="18" Scale9OriginY="25" Scale9Width="29" Scale9Height="13" ctype="ImageViewObjectData">
                    <Size X="65.0000" Y="63.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.4000" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.5000" />
                    <PreSize X="0.2743" Y="0.5888" />
                    <FileData Type="Normal" Path="games/tellTheStory/Layer-52.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1" ActionTag="-1918427552" Tag="135" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="106.7100" RightMargin="40.2900" TopMargin="32.0000" BottomMargin="32.0000" FontSize="30" LabelText="Dừng" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="90.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="196.7100" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8300" Y="0.5000" />
                    <PreSize X="0.3797" Y="0.4019" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="138" G="51" B="47" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="378.8800" Y="63.5136" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3700" Y="0.0827" />
                <PreSize X="0.2314" Y="0.1393" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_play" ActionTag="1349146379" CallBackType="Click" CallBackName="onRepeat" Tag="46" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="260.4032" RightMargin="526.6432" TopMargin="649.2200" BottomMargin="11.7800" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="207" Scale9Height="85" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="236.9536" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="Image_7" ActionTag="-1786306288" Tag="62" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="14.8907" RightMargin="157.0629" TopMargin="22.0000" BottomMargin="22.0000" LeftEage="18" RightEage="18" TopEage="25" BottomEage="25" Scale9OriginX="18" Scale9OriginY="25" Scale9Width="29" Scale9Height="13" ctype="ImageViewObjectData">
                    <Size X="65.0000" Y="63.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.3907" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.5000" />
                    <PreSize X="0.2743" Y="0.5888" />
                    <FileData Type="Normal" Path="games/tellTheStory/Layer-52.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1" ActionTag="-1347424962" Tag="63" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="83.3056" RightMargin="23.6480" TopMargin="32.0000" BottomMargin="32.0000" FontSize="30" LabelText="Nghe lại" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="130.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="213.3056" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9002" Y="0.5000" />
                    <PreSize X="0.5486" Y="0.4019" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="138" G="51" B="47" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="378.8800" Y="65.2800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3700" Y="0.0850" />
                <PreSize X="0.2314" Y="0.1393" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_stop" ActionTag="2016062597" CallBackType="Click" CallBackName="onStop" Tag="52" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="9.5000" RightMargin="777.5000" TopMargin="649.2200" BottomMargin="11.7800" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="207" Scale9Height="85" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="237.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="micro" ActionTag="-1096816820" Tag="53" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.9000" RightMargin="162.1000" TopMargin="14.5000" BottomMargin="14.5000" LeftEage="18" RightEage="18" TopEage="25" BottomEage="25" Scale9OriginX="18" Scale9OriginY="25" Scale9Width="19" Scale9Height="28" ctype="ImageViewObjectData">
                    <Size X="55.0000" Y="78.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.4000" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.5000" />
                    <PreSize X="0.2321" Y="0.7290" />
                    <FileData Type="Normal" Path="games/tellTheStory/Layer-51.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1" ActionTag="-1606344985" Tag="54" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="106.7100" RightMargin="40.2900" TopMargin="32.0000" BottomMargin="32.0000" FontSize="30" LabelText="Dừng" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="90.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="196.7100" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8300" Y="0.5000" />
                    <PreSize X="0.3797" Y="0.4019" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="138" G="51" B="47" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.0000" Y="65.2800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1250" Y="0.0850" />
                <PreSize X="0.2314" Y="0.1393" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_record" ActionTag="1925876399" CallBackType="Click" CallBackName="onRecord" Tag="45" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="9.5000" RightMargin="777.5000" TopMargin="649.2200" BottomMargin="11.7800" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="207" Scale9Height="85" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="237.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="micro" ActionTag="-1023730499" Tag="53" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.9000" RightMargin="162.1000" TopMargin="14.5000" BottomMargin="14.5000" LeftEage="18" RightEage="18" TopEage="25" BottomEage="25" Scale9OriginX="18" Scale9OriginY="25" Scale9Width="19" Scale9Height="28" ctype="ImageViewObjectData">
                    <Size X="55.0000" Y="78.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.4000" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.5000" />
                    <PreSize X="0.2321" Y="0.7290" />
                    <FileData Type="Normal" Path="games/tellTheStory/Layer-51.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1" ActionTag="1434532465" Tag="54" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="93.1900" RightMargin="30.8100" TopMargin="32.0000" BottomMargin="32.0000" FontSize="30" LabelText="Ghi âm" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="113.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="206.1900" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8700" Y="0.5000" />
                    <PreSize X="0.4768" Y="0.4019" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="138" G="51" B="47" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.0000" Y="65.2800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1250" Y="0.0850" />
                <PreSize X="0.2314" Y="0.1393" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_next" ActionTag="-2034907567" CallBackType="Click" CallBackName="onNext" Tag="66" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="772.3800" RightMargin="14.6200" TopMargin="649.2200" BottomMargin="11.7800" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="207" Scale9Height="85" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="237.0000" Y="107.0000" />
                <Children>
                  <AbstractNodeData Name="Image_7" ActionTag="105265659" Tag="67" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="11.9000" RightMargin="154.1000" TopMargin="25.5000" BottomMargin="25.5000" LeftEage="18" RightEage="18" TopEage="25" BottomEage="25" Scale9OriginX="18" Scale9OriginY="25" Scale9Width="35" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="71.0000" Y="56.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.4000" Y="53.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2000" Y="0.5000" />
                    <PreSize X="0.2996" Y="0.5234" />
                    <FileData Type="Normal" Path="games/tellTheStory/next_icon.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Text_1" ActionTag="-322798105" Tag="68" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="88.7123" RightMargin="21.2877" TopMargin="30.4164" BottomMargin="33.5836" FontSize="30" LabelText="Tiếp tục" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="127.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="215.7123" Y="55.0836" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9102" Y="0.5148" />
                    <PreSize X="0.5359" Y="0.4019" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="138" G="51" B="47" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="890.8800" Y="65.2800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8700" Y="0.0850" />
                <PreSize X="0.2314" Y="0.1393" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/tellTheStory/Vector-Smart-Object1.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="holder" ActionTag="-1286276230" Tag="75" IconVisible="False" LeftMargin="-308.8503" RightMargin="1021.8503" TopMargin="1041.8687" BottomMargin="-556.8687" LeftEage="102" RightEage="102" TopEage="93" BottomEage="93" Scale9OriginX="102" Scale9OriginY="93" Scale9Width="107" Scale9Height="97" ctype="ImageViewObjectData">
            <Size X="311.0000" Y="283.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-153.3503" Y="-415.3687" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="-0.1498" Y="-0.5408" />
            <PreSize X="0.3037" Y="0.3685" />
            <FileData Type="Normal" Path="games/tellTheStory/Layer-48-copy.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>