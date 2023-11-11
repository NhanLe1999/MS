<GameFile>
  <PropertyGroup Name="AISpeakingShareVideoPopup" Type="Layer" ID="1bdf7b82-80f4-4e4e-8bb1-ebfece27b75f" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MSAISpeakingSharePopup" Tag="81" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background_overlay" ActionTag="-491726018" Alpha="127" Tag="97" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="2000.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.9531" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="rootlayout" ActionTag="-2046287800" Tag="14" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="background_blue" ActionTag="-1972808278" Alpha="249" Tag="60" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="179.0000" RightMargin="179.0000" TopMargin="177.0000" BottomMargin="177.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="844" Scale9Height="510" ctype="ImageViewObjectData">
                <Size X="666.0000" Y="414.0000" />
                <Children>
                  <AbstractNodeData Name="background_white" ActionTag="-379138877" Alpha="249" Tag="101" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="9.9900" RightMargin="9.9900" TopMargin="8.2800" BottomMargin="8.2800" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="824" Scale9Height="490" ctype="ImageViewObjectData">
                    <Size X="646.0200" Y="397.4400" />
                    <Children>
                      <AbstractNodeData Name="lb_share_video_title" ActionTag="-999555840" Tag="114" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="128.0100" RightMargin="128.0100" TopMargin="93.2320" BottomMargin="252.2080" FontSize="38" LabelText="Let’s share this video!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="390.0000" Y="52.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="323.0100" Y="278.2080" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="1" G="196" B="248" />
                        <PrePosition X="0.5000" Y="0.7000" />
                        <PreSize X="0.6037" Y="0.1308" />
                        <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="love_icon" ActionTag="-879529898" Tag="17" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="218.0100" RightMargin="218.0100" TopMargin="-105.0000" BottomMargin="292.4400" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="184" Scale9Height="184" ctype="ImageViewObjectData">
                        <Size X="210.0000" Y="210.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="323.0100" Y="397.4400" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="0.3251" Y="0.5284" />
                        <FileData Type="Normal" Path="mjstory/AISpeaking/smile_Icon.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="share_button" ActionTag="-1977058853" CallBackType="Click" CallBackName="onShareButtonClicked" Tag="110" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="56.0100" RightMargin="56.0100" TopMargin="170.5920" BottomMargin="130.8480" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="504" Scale9Height="74" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="534.0000" Y="96.0000" />
                        <Children>
                          <AbstractNodeData Name="text_share" ActionTag="1856455792" Tag="111" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="254.7000" RightMargin="201.3000" TopMargin="29.0000" BottomMargin="29.0000" FontSize="28" LabelText="Share" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="78.0000" Y="38.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="293.7000" Y="48.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5500" Y="0.5000" />
                            <PreSize X="0.1461" Y="0.3958" />
                            <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="icon_share" ActionTag="-1023748383" Tag="107" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="199.1000" RightMargin="305.9000" TopMargin="35.5000" BottomMargin="35.5000" LeftEage="9" RightEage="9" TopEage="9" BottomEage="9" Scale9OriginX="9" Scale9OriginY="9" Scale9Width="11" Scale9Height="7" ctype="ImageViewObjectData">
                            <Size X="29.0000" Y="25.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="213.6000" Y="48.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.4000" Y="0.5000" />
                            <PreSize X="0.0543" Y="0.2604" />
                            <FileData Type="Normal" Path="mjstory/AISpeaking/vector_share_icon.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="323.0100" Y="178.8480" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.4500" />
                        <PreSize X="0.8266" Y="0.2415" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <PressedFileData Type="Normal" Path="mjstory/AISpeaking/button_share.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/AISpeaking/button_share.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="finish_button" ActionTag="-359282340" CallBackType="Click" CallBackName="onFinishButtonClicked" Tag="112" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="54.0100" RightMargin="54.0100" TopMargin="268.4520" BottomMargin="29.9880" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="508" Scale9Height="77" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="538.0000" Y="99.0000" />
                        <Children>
                          <AbstractNodeData Name="text_finish" ActionTag="-940598574" Tag="113" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="255.9000" RightMargin="202.1000" TopMargin="30.5000" BottomMargin="30.5000" FontSize="28" LabelText="Finish" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="80.0000" Y="38.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="295.9000" Y="49.5000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="1" G="196" B="248" />
                            <PrePosition X="0.5500" Y="0.5000" />
                            <PreSize X="0.1487" Y="0.3838" />
                            <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="icon_finish" ActionTag="-1767893339" Tag="106" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="200.7000" RightMargin="308.3000" TopMargin="35.0000" BottomMargin="35.0000" LeftEage="9" RightEage="9" TopEage="9" BottomEage="9" Scale9OriginX="9" Scale9OriginY="9" Scale9Width="11" Scale9Height="11" ctype="ImageViewObjectData">
                            <Size X="29.0000" Y="29.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="215.2000" Y="49.5000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.4000" Y="0.5000" />
                            <PreSize X="0.0539" Y="0.2929" />
                            <FileData Type="Normal" Path="mjstory/AISpeaking/tick_icon.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="323.0100" Y="79.4880" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.2000" />
                        <PreSize X="0.8328" Y="0.2491" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/AISpeaking/finish_button.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="close_button" ActionTag="-557975029" CallBackType="Click" CallBackName="onCloseButtonClick" Tag="103" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="594.2758" RightMargin="1.7443" BottomMargin="347.4400" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="25" Scale9Height="33" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="50.0000" Y="50.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="1.0000" />
                        <Position X="644.2758" Y="397.4400" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.9973" Y="1.0000" />
                        <PreSize X="0.0774" Y="0.1258" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Normal" Path="mjstory/AISpeaking/X.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/AISpeaking/X.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="333.0000" Y="207.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9700" Y="0.9600" />
                    <FileData Type="Normal" Path="mjstory/AISpeaking/Subtract.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6504" Y="0.5391" />
                <FileData Type="Normal" Path="mjstory/AISpeaking/Rectangle_popup_permission.png" Plist="" />
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
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>