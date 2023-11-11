<GameFile>
  <PropertyGroup Name="AISpeakingAskPermissionPopup" Type="Layer" ID="d5bb5b4e-780b-4a59-b7a8-8b9731c1cea5" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MSAISpeakingPermissionPopup" Tag="81" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background_overlay" ActionTag="746016229" Alpha="127" Tag="43" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-768.0000" RightMargin="-768.0000" TopMargin="-384.0000" BottomMargin="-384.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="2560.0000" Y="1536.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="2.5000" Y="2.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="layout_root" ActionTag="-265627347" Tag="42" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="85.0000" RightMargin="85.0000" TopMargin="124.0000" BottomMargin="124.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="854.0000" Y="520.0000" />
            <Children>
              <AbstractNodeData Name="root" ActionTag="312580420" Tag="12" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" LeftEage="288" RightEage="288" TopEage="178" BottomEage="178" Scale9OriginX="288" Scale9OriginY="178" Scale9Width="298" Scale9Height="184" ctype="PanelObjectData">
                <Size X="854.0000" Y="520.0000" />
                <Children>
                  <AbstractNodeData Name="white_image" ActionTag="-779162543" Tag="113" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" LeftEage="281" RightEage="281" TopEage="171" BottomEage="171" Scale9OriginX="281" Scale9OriginY="171" Scale9Width="292" Scale9Height="178" ctype="PanelObjectData">
                    <Size X="854.0000" Y="520.0000" />
                    <Children>
                      <AbstractNodeData Name="button_close" ActionTag="-1229686199" CallBackType="Click" CallBackName="onClose" Tag="84" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="799.0000" BottomMargin="465.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="25" Scale9Height="33" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="55.0000" Y="55.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="1.0000" />
                        <Position X="854.0000" Y="520.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="1.0000" Y="1.0000" />
                        <PreSize X="0.0644" Y="0.1058" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/AISpeaking/X.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="text_title" ActionTag="1144235174" Tag="114" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="284.0001" RightMargin="283.9999" TopMargin="24.5003" BottomMargin="434.4997" FontSize="50" LabelText="Thông báo" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="286.0000" Y="61.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="427.0001" Y="464.9997" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="1" G="196" B="248" />
                        <PrePosition X="0.5000" Y="0.8942" />
                        <PreSize X="0.3349" Y="0.1173" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="text_description" ActionTag="75435222" Tag="115" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="42.6999" RightMargin="42.7001" TopMargin="124.0021" BottomMargin="322.0020" IsCustomSize="True" FontSize="27" LabelText="Ba mẹ hãy cho phép truy cập Camera và Microphone để&#xA;Monkey giúp bé luyện tập hội thoại nhé!" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="768.6000" Y="73.9960" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="426.9999" Y="358.9999" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="1" G="196" B="248" />
                        <PrePosition X="0.5000" Y="0.6904" />
                        <PreSize X="0.9000" Y="0.1423" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="background_icon" ActionTag="-1584675090" Tag="116" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="256.2000" RightMargin="256.2000" TopMargin="234.0000" BottomMargin="130.0000" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" LeftEage="288" RightEage="288" TopEage="178" BottomEage="178" Scale9OriginX="-288" Scale9OriginY="-178" Scale9Width="576" Scale9Height="356" ctype="PanelObjectData">
                        <Size X="341.6000" Y="156.0000" />
                        <Children>
                          <AbstractNodeData Name="image_camera" ActionTag="750893291" Tag="117" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="42.4800" RightMargin="179.1200" TopMargin="18.0000" BottomMargin="18.0000" LeftEage="39" RightEage="39" TopEage="39" BottomEage="39" Scale9OriginX="39" Scale9OriginY="39" Scale9Width="42" Scale9Height="42" ctype="ImageViewObjectData">
                            <Size X="120.0000" Y="120.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="102.4800" Y="78.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.3000" Y="0.5000" />
                            <PreSize X="0.3513" Y="0.7692" />
                            <FileData Type="Normal" Path="mjstory/AISpeaking/camera.png" Plist="" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="image_audio" ActionTag="1866826441" Tag="118" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="189.1200" RightMargin="52.4800" TopMargin="28.0000" BottomMargin="28.0000" LeftEage="33" RightEage="33" TopEage="33" BottomEage="33" Scale9OriginX="33" Scale9OriginY="33" Scale9Width="34" Scale9Height="34" ctype="ImageViewObjectData">
                            <Size X="100.0000" Y="100.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="239.1200" Y="78.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.7000" Y="0.5000" />
                            <PreSize X="0.2927" Y="0.6410" />
                            <FileData Type="Normal" Path="mjstory/AISpeaking/microphone2.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="427.0000" Y="208.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.4000" />
                        <PreSize X="0.4000" Y="0.3000" />
                        <SingleColor A="255" R="255" G="255" B="255" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="button_accept" ActionTag="613583882" CallBackType="Click" CallBackName="onAccept" Tag="119" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="252.0000" RightMargin="252.0000" TopMargin="382.5000" BottomMargin="18.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="320" Scale9Height="97" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="350.0000" Y="119.0000" />
                        <Children>
                          <AbstractNodeData Name="text_accept" ActionTag="-1803230613" Tag="121" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="105.0000" RightMargin="105.0000" TopMargin="29.1700" BottomMargin="45.8300" FontSize="36" LabelText="Đồng ý" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="140.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="175.0000" Y="67.8300" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5700" />
                            <PreSize X="0.4000" Y="0.3697" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="255" G="255" B="255" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="427.0000" Y="78.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.1500" />
                        <PreSize X="0.4098" Y="0.2288" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/AISpeaking/accept_button.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="427.0000" Y="260.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="mjstory/AISpeaking/Subtract.png" Plist="" />
                    <SingleColor A="255" R="1" G="196" B="248" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="427.0000" Y="260.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="mjstory/AISpeaking/Rectangle_popup_permission.png" Plist="" />
                <SingleColor A="255" R="255" G="255" B="255" />
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
            <PreSize X="0.8340" Y="0.6771" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="line" ActionTag="-379537491" VisibleForFrame="False" Tag="197" IconVisible="False" LeftMargin="87.8203" RightMargin="86.1797" TopMargin="239.1505" BottomMargin="510.8495" LeftEage="218" RightEage="218" TopEage="5" BottomEage="5" Scale9OriginX="218" Scale9OriginY="5" Scale9Width="226" Scale9Height="8" ctype="ImageViewObjectData">
            <Size X="850.0000" Y="18.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.8203" Y="519.8495" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5008" Y="0.6769" />
            <PreSize X="0.8301" Y="0.0234" />
            <FileData Type="Normal" Path="free2paid/popup/line.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>