<GameFile>
  <PropertyGroup Name="ProfileNamePopup" Type="Layer" ID="30404660-d599-4f37-af62-4e785b67eca7" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
        <Timeline ActionTag="521911233" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
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
      <ObjectData Name="layer" CustomClassName="APProfileNameView" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="-1628935586" CallBackType="Click" CallBackName="onClose" Tag="167" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
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
          <AbstractNodeData Name="bg_ngang" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="102.4000" RightMargin="102.4000" TopMargin="76.8000" BottomMargin="76.8000" TouchEnable="True" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="45" Scale9Height="45" ctype="ImageViewObjectData">
            <Size X="819.2000" Y="614.4000" />
            <Children>
              <AbstractNodeData Name="Text_2" ActionTag="-1681810128" Tag="345" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="128.1000" RightMargin="128.1000" TopMargin="92.0240" BottomMargin="448.3760" FontSize="30" LabelText="Monkey sẽ gợi ý nội dung phù hợp &#xA;để xây dựng lộ trình học riêng cho bé" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="563.0000" Y="74.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="409.6000" Y="485.3760" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.7900" />
                <PreSize X="0.6873" Y="0.1204" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_name" ActionTag="-1129464335" Tag="85" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="109.6000" RightMargin="109.6000" TopMargin="204.4000" BottomMargin="310.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="600.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="name_icon" ActionTag="-1023820184" Tag="599" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="32.5000" RightMargin="532.5000" TopMargin="32.5000" BottomMargin="32.5000" LeftEage="33" RightEage="33" TopEage="33" BottomEage="33" Scale9OriginX="33" Scale9OriginY="33" Scale9Width="34" Scale9Height="34" ctype="ImageViewObjectData">
                    <Size X="35.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="50.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0833" Y="0.5000" />
                    <PreSize X="0.0583" Y="0.3500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/ap_icon_user.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_15" ActionTag="-417168236" Alpha="51" Tag="87" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="600.0000" Y="100.0000" />
                    <AnchorPoint />
                    <Position />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield" ActionTag="1494759529" Tag="88" IconVisible="False" PercentHeightEnable="True" PercentHeightEnabled="True" HorizontalEdge="RightEdge" LeftMargin="120.0000" TouchEnable="True" FontSize="30" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập tên con của bạn" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="480.0000" Y="100.0000" />
                    <AnchorPoint />
                    <Position X="120.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="77" G="77" B="77" />
                    <PrePosition X="0.2000" />
                    <PreSize X="0.8000" Y="1.0000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="409.6000" Y="360.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5859" />
                <PreSize X="0.7324" Y="0.1628" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_gender" ActionTag="1754113619" Tag="89" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="109.6000" RightMargin="109.6000" TopMargin="324.4000" BottomMargin="170.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="600.0000" Y="120.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_2" ActionTag="-957631117" Tag="90" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="209.0000" RightMargin="389.0000" TopMargin="20.0000" BottomMargin="20.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="2.0000" Y="80.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="210.0000" Y="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3500" Y="0.5000" />
                    <PreSize X="0.0033" Y="0.6667" />
                    <SingleColor A="255" R="0" G="0" B="0" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="gender_label" ActionTag="-252992629" Tag="91" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="60.0000" RightMargin="403.0000" TopMargin="40.5000" BottomMargin="40.5000" FontSize="32" LabelText="Giới tính" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="137.0000" Y="39.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="60.0000" Y="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="29" G="29" B="29" />
                    <PrePosition X="0.1000" Y="0.5000" />
                    <PreSize X="0.2283" Y="0.3250" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon_boy" ActionTag="1084714981" CallBackType="Click" CallBackName="onSelectBoy" Tag="92" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="285.0000" RightMargin="225.0000" TopMargin="15.0000" BottomMargin="15.0000" TouchEnable="True" LeftEage="1" RightEage="1" TopEage="1" BottomEage="1" Scale9OriginX="1" Scale9OriginY="1" Scale9Width="267" Scale9Height="266" ctype="ImageViewObjectData">
                    <Size X="90.0000" Y="90.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="330.0000" Y="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5500" Y="0.5000" />
                    <PreSize X="0.1500" Y="0.7500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/male.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon_girl" ActionTag="-1281013828" CallBackType="Click" CallBackName="onSelectGirl" Tag="93" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="430.0000" RightMargin="80.0000" TopMargin="15.0000" BottomMargin="15.0000" TouchEnable="True" LeftEage="49" RightEage="49" TopEage="33" BottomEage="33" Scale9OriginX="49" Scale9OriginY="33" Scale9Width="190" Scale9Height="221" ctype="ImageViewObjectData">
                    <Size X="90.0000" Y="90.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="475.0000" Y="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7917" Y="0.5000" />
                    <PreSize X="0.1500" Y="0.7500" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/female.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="409.6000" Y="230.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3743" />
                <PreSize X="0.7324" Y="0.1953" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="header" ActionTag="485827049" Tag="113" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" BottomMargin="534.4000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="819.2000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="346659697" Tag="114" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="169.1000" RightMargin="169.1000" TopMargin="15.5000" BottomMargin="15.5000" FontSize="40" LabelText="Tạo thông tin người học" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="481.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="409.6000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5872" Y="0.6125" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btn_close" ActionTag="-1640209626" CallBackType="Click" CallBackName="onClose" Tag="115" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="748.2400" RightMargin="10.9600" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="30" Scale9Height="38" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="778.2400" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9500" Y="0.5000" />
                    <PreSize X="0.0732" Y="0.7500" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_sidebar_close.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="1.0000" />
                <Position Y="614.4000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="1.0000" />
                <PreSize X="1.0000" Y="0.1302" />
                <SingleColor A="255" R="41" G="176" B="223" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_next" ActionTag="1981271285" CallBackType="Click" CallBackName="onNext" Tag="590" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="309.6000" RightMargin="309.6000" TopMargin="509.4000" BottomMargin="45.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0_0" ActionTag="1327501392" Tag="591" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="64.0000" RightMargin="64.0000" TopMargin="10.5000" BottomMargin="10.5000" FontSize="32" LabelText="Tiếp" HorizontalAlignmentType="HT_Right" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="72.0000" Y="39.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3600" Y="0.6500" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="896266788" VisibleForFrame="False" Tag="483" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="117.5000" RightMargin="67.5000" TopMargin="12.5000" BottomMargin="12.5000" FlipX="True" Scale9Enable="True" LeftEage="1" RightEage="1" TopEage="1" BottomEage="1" Scale9OriginX="1" Scale9OriginY="1" Scale9Width="57" Scale9Height="123" ctype="ImageViewObjectData">
                    <Size X="15.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="125.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6250" Y="0.5000" />
                    <PreSize X="0.0750" Y="0.5833" />
                    <FileData Type="Normal" Path="mjstory/new_ui/login/icon_back.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="409.6000" Y="75.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1221" />
                <PreSize X="0.2441" Y="0.0977" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8000" Y="0.8000" />
            <FileData Type="Normal" Path="mjstory/dropdown/mj_level_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>