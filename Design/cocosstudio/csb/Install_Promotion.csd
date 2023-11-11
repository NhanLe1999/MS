<GameFile>
  <PropertyGroup Name="Install_Promotion" Type="Layer" ID="0b6f6ef8-3a6a-4616-b80e-621b4e4d150e" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" ActivedAnimationName="show" />
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="60" EndIndex="95">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="layer" CustomClassName="MSInstallPromotion" Tag="31" ctype="GameLayerObjectData">
        <Size X="600.0000" Y="385.0000" />
        <Children>
          <AbstractNodeData Name="bg_ngang" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" Scale9Enable="True" LeftEage="152" RightEage="152" TopEage="109" BottomEage="109" Scale9OriginX="152" Scale9OriginY="109" Scale9Width="313" Scale9Height="359" ctype="ImageViewObjectData">
            <Size X="500.0000" Y="385.0000" />
            <Children>
              <AbstractNodeData Name="monkey" ActionTag="-493127865" Tag="71" IconVisible="False" LeftMargin="193.0005" RightMargin="70.9995" TopMargin="9.0000" BottomMargin="61.0000" FlipX="True" LeftEage="77" RightEage="77" TopEage="33" BottomEage="33" Scale9OriginX="77" Scale9OriginY="33" Scale9Width="82" Scale9Height="249" ctype="ImageViewObjectData">
                <Size X="236.0000" Y="315.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="193.0005" Y="376.0000" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3860" Y="0.9766" />
                <PreSize X="0.4720" Y="0.8182" />
                <FileData Type="Normal" Path="mjstory/popup/viral_popup_monkey.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_1" ActionTag="-1944556583" Tag="166" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="4.0000" RightMargin="6.0000" TopMargin="220.0000" BottomMargin="5.0000" LeftEage="97" RightEage="97" TopEage="32" BottomEage="32" Scale9OriginX="97" Scale9OriginY="32" Scale9Width="396" Scale9Height="131" ctype="ImageViewObjectData">
                <Size X="490.0000" Y="160.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="249.0000" Y="5.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4980" Y="0.0130" />
                <PreSize X="0.9800" Y="0.4156" />
                <FileData Type="Normal" Path="bookset.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="btfree" ActionTag="-193854663" CallBackType="Click" CallBackName="onReceive" Tag="72" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="275.0002" RightMargin="24.9998" TopMargin="25.0000" BottomMargin="300.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="24" RightEage="24" TopEage="21" BottomEage="21" Scale9OriginX="24" Scale9OriginY="21" Scale9Width="2" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="titlebtn" ActionTag="-259195062" Tag="73" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="7.0000" RightMargin="7.0000" TopMargin="15.0000" BottomMargin="15.0000" FontSize="25" LabelText="Đăng ký ngay" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="186.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="30.0000" />
                    <Scale ScaleX="0.9000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9300" Y="0.5000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="77" G="77" B="77" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.0002" Y="330.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="165" B="0" />
                <PrePosition X="0.7500" Y="0.8571" />
                <PreSize X="0.4000" Y="0.1558" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_new_selected.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="titlebtn_0" ActionTag="451667000" Tag="167" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="266.0037" RightMargin="13.9963" TopMargin="103.4996" BottomMargin="244.5004" FontSize="30" LabelText="Nhận 2 ưu đãi" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="220.0000" Y="37.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="376.0037" Y="263.0004" />
                <Scale ScaleX="0.9000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7520" Y="0.6831" />
                <PreSize X="0.4400" Y="0.0961" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="77" G="77" B="77" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btclose" ActionTag="-885235518" CallBackType="Click" CallBackName="onClose" Tag="36" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="476.5000" RightMargin="-23.5000" TopMargin="-23.5000" BottomMargin="361.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="47.0000" Y="47.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="500.0000" Y="385.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="1.0000" />
                <PreSize X="0.0940" Y="0.1221" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btreceive" ActionTag="-1657047632" VisibleForFrame="False" CallBackType="Click" CallBackName="onReceiveGift" Tag="25" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="15.0000" RightMargin="85.0000" TopMargin="349.4500" BottomMargin="11.5500" TouchEnable="True" FontSize="20" LabelText="Bạn nhận được chia sẻ? Bấm vào đây!" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="400.0000" Y="24.0000" />
                <AnchorPoint />
                <Position X="15.0000" Y="11.5500" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0300" Y="0.0300" />
                <PreSize X="0.8000" Y="0.0623" />
                <FontResource Type="Normal" Path="fonts/Montserrat-MediumItalic.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="titlebtn_0_0" ActionTag="2014205684" Tag="168" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="307.0026" RightMargin="24.9974" TopMargin="146.0346" BottomMargin="211.9654" FontSize="22" LabelText="• Giảm giá 50%" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="168.0000" Y="27.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="475.0026" Y="225.4654" />
                <Scale ScaleX="0.9000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9500" Y="0.5856" />
                <PreSize X="0.3360" Y="0.0701" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="77" G="77" B="77" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="titlebtn_0_0_0" ActionTag="692827594" Tag="169" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="184.0031" RightMargin="24.9969" TopMargin="178.5696" BottomMargin="179.4304" FontSize="22" LabelText="• Bộ 5 quyển truyện tranh" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="291.0000" Y="27.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                <Position X="475.0031" Y="192.9304" />
                <Scale ScaleX="0.9000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9500" Y="0.5011" />
                <PreSize X="0.5820" Y="0.0701" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="77" G="77" B="77" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="300.0000" Y="192.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8333" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/popup/viral_popup_bg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="Image_3_0" ActionTag="-1240402162" Tag="77" IconVisible="False" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" HorizontalEdge="LeftEdge" RightMargin="550.0000" FlipX="True" LeftEage="16" RightEage="16" TopEage="49" BottomEage="49" Scale9OriginX="16" Scale9OriginY="49" Scale9Width="18" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="50.0000" Y="385.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="25.0000" Y="192.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0417" Y="0.5000" />
            <PreSize X="0.0833" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/parent/parent_notification_item_shadow.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="Image_3" ActionTag="-1400794570" Tag="78" IconVisible="False" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" HorizontalEdge="RightEdge" LeftMargin="550.0000" LeftEage="16" RightEage="16" TopEage="49" BottomEage="49" Scale9OriginX="16" Scale9OriginY="49" Scale9Width="18" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="50.0000" Y="385.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="575.0000" Y="192.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9583" Y="0.5000" />
            <PreSize X="0.0833" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/parent/parent_notification_item_shadow.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>