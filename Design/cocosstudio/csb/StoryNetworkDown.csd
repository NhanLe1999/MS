<GameFile>
  <PropertyGroup Name="StoryNetworkDown" Type="Layer" ID="4cfea156-6fe4-4bd9-a279-8d05a90a0ffe" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000">
        <Timeline ActionTag="974546587" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="204">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="204">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="90" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-1935582594" Property="Scale">
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1826786892" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.2000" Y="1.2000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.2000" Y="1.2000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0100" Y="0.0100">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="60" EndIndex="95">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="layer" CustomClassName="StoryAlert" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="0" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="alert_layout" ActionTag="1826786892" Tag="14" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="310.5000" RightMargin="310.5000" TopMargin="185.0000" BottomMargin="185.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="403.0000" Y="398.0000" />
            <Children>
              <AbstractNodeData Name="alert_layout_ngang" ActionTag="-267048932" Tag="26" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="403.0000" Y="398.0000" />
                <Children>
                  <AbstractNodeData Name="alert_bg" ActionTag="134775550" Tag="27" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-211.0000" RightMargin="-211.0000" TopMargin="-160.9898" BottomMargin="-141.0102" LeftEage="152" RightEage="152" TopEage="109" BottomEage="109" Scale9OriginX="152" Scale9OriginY="109" Scale9Width="521" Scale9Height="482" ctype="ImageViewObjectData">
                    <Size X="825.0000" Y="700.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_1" ActionTag="1231413424" Tag="29" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="235.5000" RightMargin="235.5000" TopMargin="303.0000" BottomMargin="209.0000" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="678" Scale9Height="345" ctype="ImageViewObjectData">
                        <Size X="354.0000" Y="188.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="412.5000" Y="303.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.4329" />
                        <PreSize X="0.4291" Y="0.2686" />
                        <FileData Type="Normal" Path="mjstory/monkeystories_logo_new.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="label_message" ActionTag="81261908" Tag="30" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="83.0000" RightMargin="83.0000" TopMargin="516.0000" BottomMargin="136.0000" FontSize="20" LabelText="Could not contact the Monkey Junior server&#xA;Please make sure you are connected to the internet and try again" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="659.0000" Y="48.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="412.5000" Y="160.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="40" G="127" B="157" />
                        <PrePosition X="0.5000" Y="0.2286" />
                        <PreSize X="0.7988" Y="0.0686" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="btok" ActionTag="1939432388" CallBackType="Click" CallBackName="onOK" Tag="31" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="42.6900" RightMargin="410.3100" TopMargin="594.8656" BottomMargin="36.1344" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="342" Scale9Height="47" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="372.0000" Y="69.0000" />
                        <Children>
                          <AbstractNodeData Name="label_yes" ActionTag="-1063610650" Tag="32" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="70.5000" RightMargin="70.5000" TopMargin="21.0000" BottomMargin="21.0000" FontSize="22" LabelText="Start in Offline Mode" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="231.0000" Y="27.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="186.0000" Y="34.5000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.6210" Y="0.3913" />
                            <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="Image_4" ActionTag="53069605" Tag="734" IconVisible="False" LeftMargin="-9.8167" RightMargin="317.8167" TopMargin="9.6394" BottomMargin="28.3606" LeftEage="21" RightEage="21" TopEage="10" BottomEage="10" Scale9OriginX="21" Scale9OriginY="10" Scale9Width="22" Scale9Height="11" ctype="ImageViewObjectData">
                            <Size X="64.0000" Y="31.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="22.1833" Y="43.8606" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0596" Y="0.6357" />
                            <PreSize X="0.1720" Y="0.4493" />
                            <FileData Type="Normal" Path="mjstory/popup/networkdown_ngang_chitiet1.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="228.6900" Y="70.6344" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2772" Y="0.1009" />
                        <PreSize X="0.4509" Y="0.0986" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/networkdown_ngang_bt1.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="btcancel" ActionTag="956803855" CallBackType="Click" CallBackName="onCancel" Tag="33" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="412.5325" RightMargin="39.4675" TopMargin="595.3656" BottomMargin="36.6344" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="343" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="373.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="label_no" ActionTag="527594061" Tag="34" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="135.0000" RightMargin="135.0000" TopMargin="20.5000" BottomMargin="20.5000" FontSize="22" LabelText="Try again" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="103.0000" Y="27.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="186.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.2761" Y="0.3971" />
                            <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="Image_5" ActionTag="1071673270" Tag="735" IconVisible="False" LeftMargin="301.7894" RightMargin="-8.7894" TopMargin="11.5843" BottomMargin="25.4157" LeftEage="26" RightEage="26" TopEage="10" BottomEage="10" Scale9OriginX="26" Scale9OriginY="10" Scale9Width="28" Scale9Height="11" ctype="ImageViewObjectData">
                            <Size X="80.0000" Y="31.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="341.7894" Y="40.9157" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.9163" Y="0.6017" />
                            <PreSize X="0.2145" Y="0.4559" />
                            <FileData Type="Normal" Path="mjstory/popup/networkdown_ngang_chitiet2.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="599.0325" Y="70.6344" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.7261" Y="0.1009" />
                        <PreSize X="0.4521" Y="0.0971" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/networkdown_ngang_bt2.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="btcancel_x" ActionTag="-1935582594" CallBackType="Click" CallBackName="onClose" Tag="359" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="761.5000" RightMargin="16.5000" TopMargin="206.5000" BottomMargin="446.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="47.0000" Y="47.0000" />
                        <Children>
                          <AbstractNodeData Name="Panel_3" Visible="False" ActionTag="1784429448" CallBackType="Click" CallBackName="onClose" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-9.4000" RightMargin="-9.4000" TopMargin="-9.4000" BottomMargin="-9.4000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                            <Size X="65.8000" Y="65.8000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="23.5000" Y="23.5000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="1.4000" Y="1.4000" />
                            <SingleColor A="255" R="150" G="200" B="255" />
                            <FirstColor A="255" R="150" G="200" B="255" />
                            <EndColor A="255" R="255" G="255" B="255" />
                            <ColorVector ScaleY="1.0000" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="785.0000" Y="470.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.9515" Y="0.6714" />
                        <PreSize X="0.0570" Y="0.0671" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="titlelb" ActionTag="1529033988" Tag="35" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="272.0000" RightMargin="272.0000" TopMargin="254.9300" BottomMargin="411.0700" FontSize="28" LabelText="Couldn't connect to" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="281.0000" Y="34.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="412.5000" Y="428.0700" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="40" G="127" B="157" />
                        <PrePosition X="0.5000" Y="0.6115" />
                        <PreSize X="0.3406" Y="0.0486" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="btone" ActionTag="686343545" VisibleForFrame="False" CallBackType="Click" CallBackName="onCancel" Tag="36" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="42.4800" RightMargin="38.5200" TopMargin="594.5102" BottomMargin="36.4898" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="482" Scale9Height="49" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="744.0000" Y="69.0000" />
                        <Children>
                          <AbstractNodeData Name="label_one" ActionTag="-1578047183" Tag="37" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="291.5000" RightMargin="291.5000" TopMargin="13.0000" BottomMargin="13.0000" FontSize="35" LabelText="Try again" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="161.0000" Y="43.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="372.0000" Y="34.5000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.2164" Y="0.6232" />
                            <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="414.4800" Y="70.9898" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5024" Y="0.1014" />
                        <PreSize X="0.9018" Y="0.0986" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Normal" Path="mjstory/popup/popup_loveapp_cancel.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/popup_loveapp_cancel.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="201.5000" Y="208.9898" />
                    <Scale ScaleX="0.8000" ScaleY="0.8000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5251" />
                    <PreSize X="2.0471" Y="1.7588" />
                    <FileData Type="Normal" Path="mjstory/popup/networkdown_ngang_bg_new.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="199.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="alert_layout_doc" ActionTag="-220315894" VisibleForFrame="False" Tag="25" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="403.0000" Y="398.0000" />
                <Children>
                  <AbstractNodeData Name="alert_bg" ActionTag="-387759813" Tag="15" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-136.0000" RightMargin="-136.0000" TopMargin="-251.0000" BottomMargin="-251.0000" LeftEage="152" RightEage="152" TopEage="109" BottomEage="109" Scale9OriginX="152" Scale9OriginY="109" Scale9Width="371" Scale9Height="682" ctype="ImageViewObjectData">
                    <Size X="675.0000" Y="900.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_1" ActionTag="732383261" Tag="17" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="175.0000" RightMargin="175.0000" TopMargin="348.4970" BottomMargin="418.5030" LeftEage="100" RightEage="100" TopEage="40" BottomEage="40" Scale9OriginX="-54" Scale9OriginY="6" Scale9Width="154" Scale9Height="34" ctype="ImageViewObjectData">
                        <Size X="325.0000" Y="133.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="337.5000" Y="485.0030" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5389" />
                        <PreSize X="0.4815" Y="0.1478" />
                        <FileData Type="Normal" Path="mjstory/monkeystories_doc.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="label_message" ActionTag="294648626" Tag="18" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="156.2625" RightMargin="140.7375" TopMargin="512.9665" BottomMargin="259.0335" FontSize="27" LabelText="Could not contact the &#xA;Monkey Junior server. Please &#xA;make sure you are connected to&#xA;the internet and try again" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="378.0000" Y="128.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="345.2625" Y="323.0335" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="40" G="127" B="157" />
                        <PrePosition X="0.5115" Y="0.3589" />
                        <PreSize X="0.5600" Y="0.1422" />
                        <FontResource Type="Normal" Path="fonts/Linotte Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="btok" ActionTag="-1442239971" CallBackType="Click" CallBackName="onOK" Tag="21" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="70.9100" RightMargin="80.0900" TopMargin="670.2543" BottomMargin="141.7457" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="66" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="524.0000" Y="88.0000" />
                        <Children>
                          <AbstractNodeData Name="label_yes" ActionTag="1530009243" Tag="22" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="131.0000" RightMargin="131.0000" TopMargin="29.0000" BottomMargin="29.0000" FontSize="25" LabelText="Start in Offline Mode" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="262.0000" Y="30.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="262.0000" Y="44.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.5000" Y="0.3409" />
                            <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="Image_3" ActionTag="-1756530889" Tag="733" IconVisible="False" LeftMargin="-11.8017" RightMargin="448.8017" TopMargin="11.7553" BottomMargin="34.2447" LeftEage="28" RightEage="28" TopEage="13" BottomEage="13" Scale9OriginX="28" Scale9OriginY="13" Scale9Width="31" Scale9Height="16" ctype="ImageViewObjectData">
                            <Size X="87.0000" Y="42.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="31.6983" Y="55.2447" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.0605" Y="0.6278" />
                            <PreSize X="0.1660" Y="0.4773" />
                            <FileData Type="Normal" Path="mjstory/popup/networkdown_doc_2.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="332.9100" Y="185.7457" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4932" Y="0.2064" />
                        <PreSize X="0.7763" Y="0.0978" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Normal" Path="mjstory/popup/networkdown_doc_bt2.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/networkdown_doc_bt2.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="btcancel" ActionTag="-283757626" CallBackType="Click" CallBackName="onCancel" Tag="19" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="71.7875" RightMargin="79.2125" TopMargin="754.5100" BottomMargin="54.4900" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="69" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="524.0000" Y="91.0000" />
                        <Children>
                          <AbstractNodeData Name="label_no" ActionTag="-1230589580" Tag="20" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="203.5000" RightMargin="203.5000" TopMargin="30.5000" BottomMargin="30.5000" FontSize="25" LabelText="Try again" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="117.0000" Y="30.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="262.0000" Y="45.5000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.2233" Y="0.3297" />
                            <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                          <AbstractNodeData Name="Image_2" ActionTag="-1298999896" Tag="732" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="450.4844" RightMargin="-6.4844" TopMargin="21.2549" BottomMargin="38.7451" LeftEage="26" RightEage="26" TopEage="10" BottomEage="10" Scale9OriginX="26" Scale9OriginY="10" Scale9Width="28" Scale9Height="11" ctype="ImageViewObjectData">
                            <Size X="80.0000" Y="31.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="490.4844" Y="54.2451" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.9360" Y="0.5961" />
                            <PreSize X="0.1527" Y="0.3407" />
                            <FileData Type="Normal" Path="mjstory/popup/networkdown_doc_1.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" />
                        <Position X="333.7875" Y="54.4900" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4945" Y="0.0605" />
                        <PreSize X="0.7763" Y="0.1011" />
                        <TextColor A="255" R="255" G="255" B="255" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Normal" Path="mjstory/popup/networkdown_doc_bt1.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/networkdown_doc_bt1.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="titlelb" ActionTag="1037238513" Tag="23" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="197.0000" RightMargin="197.0000" TopMargin="273.5927" BottomMargin="592.4073" FontSize="28" LabelText="Couldn't connect to" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="281.0000" Y="34.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="337.5000" Y="609.4073" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="40" G="127" B="157" />
                        <PrePosition X="0.5000" Y="0.6771" />
                        <PreSize X="0.4163" Y="0.0378" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="201.5000" Y="199.0000" />
                    <Scale ScaleX="0.9500" ScaleY="0.9500" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.6749" Y="2.2613" />
                    <FileData Type="Normal" Path="mjstory/popup/networkdown_doc_bg.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="199.0000" />
                <Scale ScaleX="0.9000" ScaleY="0.9000" />
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
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.3936" Y="0.5182" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>