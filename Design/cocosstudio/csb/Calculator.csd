<GameFile>
  <PropertyGroup Name="Calculator" Type="Layer" ID="9c7968db-e2f1-42a1-a43d-e3ca108538d9" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="35" Speed="1.0000" ActivedAnimationName="anim_shake">
        <Timeline ActionTag="-176103420" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="10" X="-1.0000" Y="-1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="20" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="-1.0000" Y="-1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="35" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="anim_shake" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="255" B="255" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJCalculator" Tag="84" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="715747623" Tag="85" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="wrapper" ActionTag="-10203496" Tag="79" IconVisible="True" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="512.0000" RightMargin="512.0000" TopMargin="345.6000" BottomMargin="422.4000" ctype="SingleNodeObjectData">
                <Size X="0.0000" Y="0.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-176103420" Tag="87" IconVisible="False" LeftMargin="-292.5000" RightMargin="-222.5000" TopMargin="-301.5000" BottomMargin="-261.5000" TouchEnable="True" LeftEage="169" RightEage="169" TopEage="185" BottomEage="185" Scale9OriginX="169" Scale9OriginY="185" Scale9Width="177" Scale9Height="193" ctype="ImageViewObjectData">
                    <Size X="515.0000" Y="563.0000" />
                    <Children>
                      <AbstractNodeData Name="Button_1" ActionTag="304922310" CallBackType="Click" CallBackName="onClose" Tag="292" IconVisible="False" LeftMargin="455.0000" RightMargin="-40.0000" TopMargin="-12.0000" BottomMargin="475.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="70" Scale9Height="78" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="100.0000" Y="100.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="505.0000" Y="525.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.9806" Y="0.9325" />
                        <PreSize X="0.1942" Y="0.1776" />
                        <TextColor A="255" R="38" G="176" B="230" />
                        <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                        <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                        <NormalFileData Type="Normal" Path="mjstory/pay/pay_popup_close_button.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="calc_title" ActionTag="-1454735738" Tag="111" IconVisible="False" VerticalEdge="TopEdge" LeftMargin="103.0000" RightMargin="28.0000" TopMargin="67.0000" BottomMargin="452.0000" FontSize="18" LabelText="Grown up only: &#xA;Complete this math problem to continue" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="384.0000" Y="44.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="295.0000" Y="474.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="52" G="197" B="237" />
                        <PrePosition X="0.5728" Y="0.8419" />
                        <PreSize X="0.7456" Y="0.0782" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_8" ActionTag="401504688" CallBackType="Click" CallBackName="onNumPad" Tag="117" IconVisible="False" LeftMargin="107.5000" RightMargin="332.5000" TopMargin="439.0000" BottomMargin="56.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-1750874231" Tag="101" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="24.5000" RightMargin="24.5000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="26.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3467" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="145.0000" Y="90.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2816" Y="0.1599" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="formular" ActionTag="2123300471" Tag="112" IconVisible="False" LeftMargin="105.0001" RightMargin="29.9999" TopMargin="141.2957" BottomMargin="321.7043" Scale9Enable="True" LeftEage="24" RightEage="24" TopEage="22" BottomEage="22" Scale9OriginX="24" Scale9OriginY="22" Scale9Width="27" Scale9Height="24" ctype="ImageViewObjectData">
                        <Size X="380.0000" Y="100.0000" />
                        <Children>
                          <AbstractNodeData Name="label_formula" ActionTag="-2074730870" Tag="113" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="114.5000" RightMargin="114.5000" TopMargin="28.0000" BottomMargin="28.0000" FontSize="36" LabelText="9 + 6 = 5" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="151.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="190.0000" Y="50.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3974" Y="0.4400" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="295.0001" Y="371.7043" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5728" Y="0.6602" />
                        <PreSize X="0.7379" Y="0.1776" />
                        <FileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_9" ActionTag="1190146978" CallBackType="Click" CallBackName="onNumPad" Tag="118" IconVisible="False" LeftMargin="207.5000" RightMargin="232.5000" TopMargin="439.0000" BottomMargin="56.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-743751113" Tag="119" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.5000" RightMargin="25.5000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="9" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="24.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3200" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="245.0000" Y="90.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4757" Y="0.1599" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_3" ActionTag="-1798797144" CallBackType="Click" CallBackName="onNumPad" Tag="120" IconVisible="False" LeftMargin="407.5002" RightMargin="32.4998" TopMargin="269.0001" BottomMargin="225.9999" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-728325262" Tag="121" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.0000" RightMargin="26.0000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="3" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="23.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3067" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="445.0002" Y="259.9999" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.8641" Y="0.4618" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_7" ActionTag="1055691198" CallBackType="Click" CallBackName="onNumPad" Tag="122" IconVisible="False" LeftMargin="407.5002" RightMargin="32.4998" TopMargin="354.0001" BottomMargin="140.9999" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-2092483011" Tag="123" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.5000" RightMargin="25.5000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="7" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="24.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3200" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="445.0002" Y="174.9999" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.8641" Y="0.3108" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_0" ActionTag="911175133" CallBackType="Click" CallBackName="onNumPad" Tag="124" IconVisible="False" LeftMargin="107.5000" RightMargin="332.5000" TopMargin="269.0002" BottomMargin="225.9998" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="1690407497" Tag="125" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="24.5000" RightMargin="24.5000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="0" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="26.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3467" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="145.0000" Y="259.9998" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2816" Y="0.4618" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_4" ActionTag="-723601596" CallBackType="Click" CallBackName="onNumPad" Tag="126" IconVisible="False" LeftMargin="107.5000" RightMargin="332.5000" TopMargin="354.0001" BottomMargin="140.9999" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-1846373033" Tag="127" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="23.5000" RightMargin="23.5000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="4" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="28.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3733" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="145.0000" Y="174.9999" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.2816" Y="0.3108" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_5" ActionTag="229768517" CallBackType="Click" CallBackName="onNumPad" Tag="128" IconVisible="False" LeftMargin="207.5000" RightMargin="232.5000" TopMargin="354.0001" BottomMargin="140.9999" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-1869060114" Tag="129" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.0000" RightMargin="26.0000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="23.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3067" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="245.0000" Y="174.9999" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4757" Y="0.3108" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_6" ActionTag="-292361189" CallBackType="Click" CallBackName="onNumPad" Tag="130" IconVisible="False" LeftMargin="307.5001" RightMargin="132.4999" TopMargin="354.0001" BottomMargin="140.9999" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-1248417735" Tag="131" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="6" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="25.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3333" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="345.0001" Y="174.9999" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.6699" Y="0.3108" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_1" ActionTag="-681786532" CallBackType="Click" CallBackName="onNumPad" Tag="132" IconVisible="False" LeftMargin="207.5000" RightMargin="232.5000" TopMargin="269.0002" BottomMargin="225.9998" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-1985581313" Tag="133" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="30.5000" RightMargin="30.5000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="1" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="14.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.1867" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="245.0000" Y="259.9998" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4757" Y="0.4618" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_2" ActionTag="-1022488791" CallBackType="Click" CallBackName="onNumPad" Tag="134" IconVisible="False" LeftMargin="307.5001" RightMargin="132.4999" TopMargin="269.0002" BottomMargin="225.9998" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="142970508" Tag="135" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="26.0000" RightMargin="26.0000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="36" LabelText="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="23.0000" Y="44.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.3067" Y="0.6471" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="345.0001" Y="259.9998" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.6699" Y="0.4618" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_clear" ActionTag="-1841138410" CallBackType="Click" CallBackName="onClear" Tag="136" IconVisible="False" LeftMargin="307.5000" RightMargin="132.5000" TopMargin="439.0000" BottomMargin="56.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Image_17" ActionTag="444264023" Tag="138" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="17.5000" RightMargin="17.5000" TopMargin="24.0000" BottomMargin="24.0000" FlipX="True" LeftEage="13" RightEage="13" TopEage="6" BottomEage="6" Scale9OriginX="13" Scale9OriginY="6" Scale9Width="14" Scale9Height="8" ctype="ImageViewObjectData">
                            <Size X="40.0000" Y="20.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.5333" Y="0.2941" />
                            <FileData Type="Normal" Path="mjstory/lesson/lesson_next.png" Plist="" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="345.0000" Y="90.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.6699" Y="0.1599" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="numpad_7_0" ActionTag="1544889913" CallBackType="Click" CallBackName="onOK" Tag="139" IconVisible="False" LeftMargin="407.5001" RightMargin="32.4999" TopMargin="439.0000" BottomMargin="56.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="46" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                        <Size X="75.0000" Y="68.0000" />
                        <Children>
                          <AbstractNodeData Name="Text_1_0" ActionTag="-1564122870" Tag="140" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="10.5000" RightMargin="10.5000" TopMargin="14.5000" BottomMargin="14.5000" FontSize="32" LabelText="OK" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="54.0000" Y="39.0000" />
                            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                            <Position X="37.5000" Y="34.0000" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="255" G="255" B="255" />
                            <PrePosition X="0.5000" Y="0.5000" />
                            <PreSize X="0.7200" Y="0.5735" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="445.0001" Y="90.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.8641" Y="0.1599" />
                        <PreSize X="0.1456" Y="0.1208" />
                        <TextColor A="255" R="65" G="65" B="70" />
                        <NormalFileData Type="Normal" Path="mjstory/popup/calculator_number.png" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="-35.0000" Y="20.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="mjstory/popup/calculator_bg_new.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="512.0000" Y="422.4000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5500" />
                <PreSize X="0.0000" Y="0.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>