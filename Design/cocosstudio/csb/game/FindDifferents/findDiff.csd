<GameFile>
  <PropertyGroup Name="findDiff" Type="Layer" ID="36e77a87-05b8-4071-b758-d8aa78be64dc" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="FindDifferents" Tag="66" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg_1" CanEdit="False" ActionTag="1514423050" Tag="67" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-338.0000" RightMargin="-338.0000" ctype="SpriteObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.6602" Y="1.0000" />
            <FileData Type="Normal" Path="games/findDiff/bg.jpg" Plist="" />
            <BlendFunc Src="770" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="root_layout" ActionTag="288651878" Tag="70" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="question" ActionTag="-1799349578" Tag="71" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="246.7000" RightMargin="144.3000" TopMargin="18.0900" BottomMargin="603.9100" ctype="SpriteObjectData">
                <Size X="633.0000" Y="146.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="1119312221" Tag="75" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="276.0000" RightMargin="276.0000" TopMargin="61.5000" BottomMargin="61.5000" FontSize="20" LabelText="Question" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="81.0000" Y="23.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="316.5000" Y="73.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1280" Y="0.1575" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="563.2000" Y="676.9100" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5500" Y="0.8814" />
                <PreSize X="0.6182" Y="0.1901" />
                <FileData Type="Normal" Path="games/findDiff/question.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_0" CanEdit="False" ActionTag="-1142048958" Tag="80" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="23.2400" RightMargin="514.7600" TopMargin="199.8000" BottomMargin="46.2000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="486.0000" Y="522.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-636870355" Tag="78" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="6.5000" RightMargin="6.5000" TopMargin="7.5000" BottomMargin="7.5000" LeftEage="125" RightEage="125" TopEage="134" BottomEage="134" Scale9OriginX="125" Scale9OriginY="134" Scale9Width="223" Scale9Height="239" ctype="ImageViewObjectData">
                    <Size X="473.0000" Y="507.0000" />
                    <Children>
                      <AbstractNodeData Name="ring_0" ActionTag="188842019" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="101" IconVisible="False" LeftMargin="-7.2922" RightMargin="330.2922" TopMargin="306.8788" BottomMargin="50.1212" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="67.7078" Y="125.1212" />
                        <Scale ScaleX="0.8068" ScaleY="0.5689" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.1431" Y="0.2468" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="ring_1" ActionTag="-423030715" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="99" IconVisible="False" LeftMargin="313.4038" RightMargin="9.5962" TopMargin="205.1778" BottomMargin="151.8222" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="388.4038" Y="226.8222" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.8211" Y="0.4474" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="ring_2" ActionTag="-1061297034" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="103" IconVisible="False" LeftMargin="217.7162" RightMargin="105.2838" TopMargin="285.8058" BottomMargin="71.1942" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="292.7162" Y="146.1942" />
                        <Scale ScaleX="0.7324" ScaleY="0.5541" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.6189" Y="0.2884" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="ring_3" ActionTag="-550407759" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="106" IconVisible="False" LeftMargin="173.2660" RightMargin="149.7340" TopMargin="128.4808" BottomMargin="228.5192" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="248.2660" Y="303.5192" />
                        <Scale ScaleX="1.1015" ScaleY="0.6324" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5249" Y="0.5987" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="243.0000" Y="261.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9733" Y="0.9713" />
                    <FileData Type="Normal" Path="games/findDifferent/Img100.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="frame_2" CanEdit="False" ActionTag="1368899254" Tag="68" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" ctype="SpriteObjectData">
                    <Size X="486.0000" Y="522.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="243.0000" Y="261.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="games/findDiff/frame.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="266.2400" Y="307.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2600" Y="0.4000" />
                <PreSize X="0.4746" Y="0.6797" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_1" CanEdit="False" ActionTag="250677793" Tag="82" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="514.7600" RightMargin="23.2400" TopMargin="199.8000" BottomMargin="46.2000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="486.0000" Y="522.0000" />
                <Children>
                  <AbstractNodeData Name="Image_2" ActionTag="-475665725" Tag="83" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="6.5000" RightMargin="6.5000" TopMargin="7.5000" BottomMargin="7.5000" LeftEage="125" RightEage="125" TopEage="134" BottomEage="134" Scale9OriginX="125" Scale9OriginY="134" Scale9Width="223" Scale9Height="239" ctype="ImageViewObjectData">
                    <Size X="473.0000" Y="507.0000" />
                    <Children>
                      <AbstractNodeData Name="ring_2" ActionTag="1964655443" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="104" IconVisible="False" LeftMargin="219.6563" RightMargin="103.3438" TopMargin="299.2409" BottomMargin="57.7591" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="294.6563" Y="132.7591" />
                        <Scale ScaleX="0.7324" ScaleY="0.5541" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.6230" Y="0.2619" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="ring_1" ActionTag="-29877666" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="98" IconVisible="False" LeftMargin="313.2452" RightMargin="9.7548" TopMargin="193.7253" BottomMargin="163.2747" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="388.2452" Y="238.2747" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.8208" Y="0.4700" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="ring_0" ActionTag="-1121316549" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="100" IconVisible="False" LeftMargin="-7.0707" RightMargin="330.0707" TopMargin="296.9153" BottomMargin="60.0847" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="67.9293" Y="135.0847" />
                        <Scale ScaleX="0.8068" ScaleY="0.5689" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.1436" Y="0.2664" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="ring_3" ActionTag="1858868252" Alpha="0" CallBackType="Click" CallBackName="onClickHolder" Tag="105" IconVisible="False" LeftMargin="137.0989" RightMargin="185.9011" TopMargin="130.9768" BottomMargin="226.0232" TouchEnable="True" LeftEage="82" RightEage="82" TopEage="82" BottomEage="82" Scale9OriginX="82" Scale9OriginY="82" Scale9Width="86" Scale9Height="86" ctype="ImageViewObjectData">
                        <Size X="150.0000" Y="150.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="212.0989" Y="301.0232" />
                        <Scale ScaleX="1.1015" ScaleY="0.6324" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4484" Y="0.5937" />
                        <PreSize X="0.3171" Y="0.2959" />
                        <FileData Type="Normal" Path="games/findDiff/ring2.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="243.0000" Y="261.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9733" Y="0.9713" />
                    <FileData Type="Normal" Path="games/findDifferent/Img200.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="frame_2_0" CanEdit="False" ActionTag="-1536223171" Tag="84" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" ctype="SpriteObjectData">
                    <Size X="486.0000" Y="522.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="243.0000" Y="261.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="games/findDiff/frame.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="757.7600" Y="307.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7400" Y="0.4000" />
                <PreSize X="0.4746" Y="0.6797" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="speaker" ActionTag="-2070271376" CallBackType="Click" CallBackName="onClickSpeaker" Tag="140" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="65.3800" RightMargin="843.6200" TopMargin="30.8200" BottomMargin="622.1800" TouchEnable="True" FontSize="14" ButtonText="Button" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="85" Scale9Height="93" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="115.0000" Y="115.0000" />
            <Children>
              <AbstractNodeData Name="Image_1" ActionTag="2136016946" CallBackType="Click" Tag="141" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="18.0000" RightMargin="18.0000" TopMargin="19.5000" BottomMargin="19.5000" LeftEage="22" RightEage="22" TopEage="21" BottomEage="21" Scale9OriginX="22" Scale9OriginY="21" Scale9Width="35" Scale9Height="34" ctype="ImageViewObjectData">
                <Size X="79.0000" Y="76.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="57.5000" Y="57.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6870" Y="0.6609" />
                <FileData Type="Normal" Path="games/findDiff/loa0.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="122.8800" Y="679.6800" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1200" Y="0.8850" />
            <PreSize X="0.1123" Y="0.1497" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Normal" Path="games/findDiff/loa1.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>