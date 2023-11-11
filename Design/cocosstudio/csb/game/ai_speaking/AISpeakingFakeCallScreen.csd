<GameFile>
  <PropertyGroup Name="AISpeakingFakeCallScreen" Type="Layer" ID="2350f4b4-3853-4998-850d-7241adefd78e" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="AISpeakingFakeCallScreen" Tag="81" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="959243707" Tag="91" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="2000.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="decor" ActionTag="-1745324037" Tag="558" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="488.0000" RightMargin="488.0000" LeftEage="337" RightEage="337" TopEage="253" BottomEage="253" Scale9OriginX="337" Scale9OriginY="253" Scale9Width="350" Scale9Height="262" ctype="ImageViewObjectData">
                <Size X="1024.0000" Y="768.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="1000.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.5120" Y="1.0000" />
                <FileData Type="Normal" Path="games/ai_speaking/background.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.9531" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="start_layout" Visible="False" ActionTag="-2046287800" Tag="14" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="actor" ActionTag="-1363748092" Tag="153" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="384.0000" RightMargin="384.0000" TopMargin="183.0000" BottomMargin="330.0000" LeftEage="83" RightEage="83" TopEage="83" BottomEage="83" Scale9OriginX="83" Scale9OriginY="83" Scale9Width="90" Scale9Height="89" ctype="ImageViewObjectData">
                <Size X="256.0000" Y="255.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="457.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5957" />
                <PreSize X="0.2500" Y="0.3320" />
                <FileData Type="Normal" Path="games/ai_speaking/avt_adin.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="calling_text" ActionTag="729254207" Tag="155" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="377.5000" RightMargin="377.5000" TopMargin="520.9030" BottomMargin="188.0970" FontSize="48" LabelText="Call issac..." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="269.0000" Y="59.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="217.5970" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="71" G="71" B="71" />
                <PrePosition X="0.5000" Y="0.2833" />
                <PreSize X="0.2627" Y="0.0768" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="call_button" ActionTag="-1731221971" CallBackType="Click" CallBackName="onCallButtonClicked" Tag="62" IconVisible="False" LeftMargin="388.7558" RightMargin="394.2442" TopMargin="537.0000" BottomMargin="131.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="33" RightEage="32" TopEage="11" BottomEage="11" Scale9OriginX="33" Scale9OriginY="11" Scale9Width="11" Scale9Height="90" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="241.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="call_text" ActionTag="304940277" Tag="33" IconVisible="False" LeftMargin="54.0000" RightMargin="50.0000" TopMargin="24.0000" BottomMargin="42.0000" FontSize="28" LabelText="Call issac" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="137.0000" Y="34.0000" />
                    <Children>
                      <AbstractNodeData Name="call_icon" ActionTag="214445313" Tag="154" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-24.9999" RightMargin="139.9999" TopMargin="9.4988" BottomMargin="3.5012" LeftEage="7" RightEage="7" TopEage="6" BottomEage="6" Scale9OriginX="7" Scale9OriginY="6" Scale9Width="8" Scale9Height="9" ctype="ImageViewObjectData">
                        <Size X="22.0000" Y="21.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="-13.9999" Y="14.0012" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="-0.1022" Y="0.4118" />
                        <PreSize X="0.1606" Y="0.6176" />
                        <FileData Type="Normal" Path="games/ai_speaking/call_icon.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="122.5000" Y="59.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5083" Y="0.5900" />
                    <PreSize X="0.5685" Y="0.3400" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="509.2558" Y="181.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4973" Y="0.2357" />
                <PreSize X="0.2354" Y="0.1302" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="games/ai_speaking/text_background.png" Plist="" />
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
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="end_layout" ActionTag="421057884" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="actor" ActionTag="-1389264192" Tag="39" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="384.0000" RightMargin="384.0000" TopMargin="183.0000" BottomMargin="330.0000" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="226" Scale9Height="225" ctype="ImageViewObjectData">
                <Size X="256.0000" Y="255.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="457.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5957" />
                <PreSize X="0.2500" Y="0.3320" />
                <FileData Type="Normal" Path="games/ai_speaking/avt_adin.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="call_end_layout" ActionTag="1912073596" Tag="40" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="768.0000" />
                <Children>
                  <AbstractNodeData Name="call_end_text" ActionTag="756958042" Tag="41" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="378.0000" RightMargin="378.0000" TopMargin="476.5072" BottomMargin="232.4928" FontSize="48" LabelText="Call Ended" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="268.0000" Y="59.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="261.9928" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="59" B="48" />
                    <PrePosition X="0.5000" Y="0.3411" />
                    <PreSize X="0.2617" Y="0.0768" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="call_time" ActionTag="-1378868113" Tag="42" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="473.0000" RightMargin="473.0000" TopMargin="540.2135" BottomMargin="194.7865" FontSize="27" LabelText="05:30" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="78.0000" Y="33.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="211.2865" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="121" G="121" B="121" />
                    <PrePosition X="0.5000" Y="0.2751" />
                    <PreSize X="0.0762" Y="0.0430" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
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
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="back_button" ActionTag="-1903172073" CallBackType="Click" CallBackName="onBackButtonClicked" Tag="629" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="30.7200" RightMargin="929.2800" TopMargin="16.0000" BottomMargin="688.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="-15" Scale9OriginY="-11" Scale9Width="30" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="64.0000" Y="64.0000" />
            <Children>
              <AbstractNodeData Name="icon_close" ActionTag="-804987772" Tag="630" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="4.0000" RightMargin="4.0000" TopMargin="4.0000" BottomMargin="4.0000" LeftEage="11" RightEage="11" TopEage="11" BottomEage="11" Scale9OriginX="11" Scale9OriginY="11" Scale9Width="34" Scale9Height="34" ctype="ImageViewObjectData">
                <Size X="56.0000" Y="56.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="32.0000" Y="32.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8750" Y="0.8750" />
                <FileData Type="Normal" Path="games/ai_speaking/back_button.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="0.5000" />
            <Position X="30.7200" Y="720.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0300" Y="0.9375" />
            <PreSize X="0.0625" Y="0.0833" />
            <TextColor A="255" R="65" G="65" B="70" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>