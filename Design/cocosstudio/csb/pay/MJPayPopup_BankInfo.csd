<GameFile>
  <PropertyGroup Name="MJPayPopup_BankInfo" Type="Layer" ID="e2fe02e8-1999-42fc-99cf-d72b1e61ee2e" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MJPayPopup_BankInfo" Tag="7" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="-219944786" Tag="28" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="26" G="26" B="26" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="popup" ActionTag="-589916819" Tag="9" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="162.0000" RightMargin="162.0000" TopMargin="84.0000" BottomMargin="84.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="700.0000" Y="600.0000" />
            <Children>
              <AbstractNodeData Name="Image_1" ActionTag="1177903880" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                <Size X="700.0000" Y="600.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="300.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_background.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_3" ActionTag="603299946" Tag="60" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="40.0000" RightMargin="225.0000" TopMargin="-30.0000" BottomMargin="570.0000" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                <Size X="435.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="text_pricetag" ActionTag="-1378886020" Tag="61" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="39.5000" RightMargin="39.5000" TopMargin="13.0000" BottomMargin="13.0000" FontSize="28" LabelText="Gói 1 tháng: 100.000 VND" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="356.0000" Y="34.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="217.5000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.8184" Y="0.5667" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="257.5000" Y="600.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3679" Y="1.0000" />
                <PreSize X="0.6214" Y="0.1000" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_background.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_2_0_0" ActionTag="1518907491" CallBackType="Click" CallBackName="onSwitch" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="460.0000" RightMargin="40.0000" TopMargin="-30.0000" BottomMargin="570.0000" TouchEnable="True" FontSize="24" Scale9Enable="True" LeftEage="8" RightEage="8" TopEage="11" BottomEage="11" Scale9OriginX="8" Scale9OriginY="11" Scale9Width="9" Scale9Height="23" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Text_3" ActionTag="-1305720580" Tag="59" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="48.5000" RightMargin="48.5000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="28" LabelText="Đổi gói " OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="103.0000" Y="36.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5150" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="560.0000" Y="600.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8000" Y="1.0000" />
                <PreSize X="0.2857" Y="0.1000" />
                <FontResource Type="Normal" Path="fonts/Linotte Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/pay/pay_popup_button_blue.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_9" ActionTag="-366266619" Tag="27" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="178.0000" RightMargin="178.0000" TopMargin="78.0200" BottomMargin="477.9800" FontSize="36" LabelText="Thông tin của bạn" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="344.0000" Y="44.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="499.9800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8333" />
                <PreSize X="0.4914" Y="0.0733" />
                <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_2" ActionTag="-84379410" Tag="51" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="169.0200" BottomMargin="370.9800" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                <Size X="600.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_7" ActionTag="-1983161166" Tag="99" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" RightMargin="500.0000" Scale9Enable="True" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                    <Size X="100.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_11" ActionTag="764219387" Tag="103" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.0000" RightMargin="20.0000" LeftEage="19" RightEage="19" TopEage="19" BottomEage="19" Scale9OriginX="19" Scale9OriginY="19" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
                        <Size X="60.0000" Y="60.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6000" Y="1.0000" />
                        <FileData Type="Normal" Path="mjstory/pay/pay_popup_icon_person.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0833" Y="0.5000" />
                    <PreSize X="0.1667" Y="1.0000" />
                    <FileData Type="Normal" Path="mjstory/pay/pay_popup_icon_background_blue.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield_name" ActionTag="-696702603" CallBackType="Event" CallBackName="onTextFieldEvent" Tag="107" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="120.0000" RightMargin="80.0000" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Họ và tên" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="400.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="320.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="30" G="144" B="255" />
                    <PrePosition X="0.5333" Y="0.5000" />
                    <PreSize X="0.6667" Y="0.6667" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="400.9800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6683" />
                <PreSize X="0.8571" Y="0.1000" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_text_background_blue.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_1_0" ActionTag="-217099494" VisibleForFrame="False" Tag="53" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="50.0000" RightMargin="100.0000" TopMargin="452.5000" BottomMargin="112.5000" IsCustomSize="True" FontSize="20" LabelText="* Mỗi mã kích hoạt được phép sử dụng trên 2 thiết bị" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="550.0000" Y="35.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="130.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.4643" Y="0.2167" />
                <PreSize X="0.7857" Y="0.0583" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_2" ActionTag="2026824160" CallBackType="Click" CallBackName="onBack" Tag="54" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="85.0000" RightMargin="365.0000" TopMargin="510.0000" BottomMargin="30.0000" TouchEnable="True" FontSize="24" Scale9Enable="True" LeftEage="8" RightEage="8" TopEage="11" BottomEage="11" Scale9OriginX="8" Scale9OriginY="11" Scale9Width="9" Scale9Height="23" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Text_3" ActionTag="164541311" Tag="55" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="28" LabelText="Quay lại" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="116.0000" Y="36.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="125.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4640" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="210.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3000" Y="0.1000" />
                <PreSize X="0.3571" Y="0.1000" />
                <FontResource Type="Normal" Path="fonts/Linotte Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/pay/pay_popup_button_grey.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_2_0" ActionTag="-600269730" CallBackType="Click" CallBackName="onNext" Tag="56" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="365.0000" RightMargin="85.0000" TopMargin="510.0000" BottomMargin="30.0000" TouchEnable="True" FontSize="24" Scale9Enable="True" LeftEage="8" RightEage="8" TopEage="11" BottomEage="11" Scale9OriginX="8" Scale9OriginY="11" Scale9Width="9" Scale9Height="23" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="250.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Text_3" ActionTag="-1104512879" Tag="57" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="12.0000" BottomMargin="12.0000" FontSize="28" LabelText="Tiếp tục " OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="116.0000" Y="36.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="125.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4640" Y="0.6000" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="0" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="490.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7000" Y="0.1000" />
                <PreSize X="0.3571" Y="0.1000" />
                <FontResource Type="Normal" Path="fonts/Linotte Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/pay/pay_popup_button_orange.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_2_0" ActionTag="613784845" Tag="96" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="270.0000" BottomMargin="270.0000" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                <Size X="600.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_7_0" ActionTag="-834464387" Tag="100" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" RightMargin="500.0000" Scale9Enable="True" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                    <Size X="100.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_11_0" ActionTag="-1517427923" Tag="104" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.0000" RightMargin="20.0000" LeftEage="19" RightEage="19" TopEage="19" BottomEage="19" Scale9OriginX="19" Scale9OriginY="19" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
                        <Size X="60.0000" Y="60.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6000" Y="1.0000" />
                        <FileData Type="Normal" Path="mjstory/pay/pay_popup_icon_phone.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0833" Y="0.5000" />
                    <PreSize X="0.1667" Y="1.0000" />
                    <FileData Type="Normal" Path="mjstory/pay/pay_popup_icon_background_blue.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield_phone" ActionTag="578348691" CallBackType="Event" CallBackName="onTextFieldEvent" Tag="108" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="120.0000" RightMargin="80.0000" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Số điện thoại" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="400.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="320.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="30" G="144" B="255" />
                    <PrePosition X="0.5333" Y="0.5000" />
                    <PreSize X="0.6667" Y="0.6667" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="300.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8571" Y="0.1000" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_text_background_blue.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_2_1" ActionTag="-868061695" Tag="97" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="367.0200" BottomMargin="172.9800" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                <Size X="600.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_7_0_0" ActionTag="-1449160064" Tag="101" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" RightMargin="500.0000" Scale9Enable="True" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                    <Size X="100.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_11_0_0" ActionTag="1198706130" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.0000" RightMargin="20.0000" LeftEage="19" RightEage="19" TopEage="19" BottomEage="19" Scale9OriginX="19" Scale9OriginY="19" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
                        <Size X="60.0000" Y="60.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="30.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6000" Y="1.0000" />
                        <FileData Type="Normal" Path="mjstory/pay/pay_popup_icon_mail.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0833" Y="0.5000" />
                    <PreSize X="0.1667" Y="1.0000" />
                    <FileData Type="Normal" Path="mjstory/pay/pay_popup_icon_background_blue.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield_mail" ActionTag="1160361656" CallBackType="Event" CallBackName="onTextFieldEvent" Tag="109" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="120.0000" RightMargin="80.0000" TopMargin="10.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Email" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="400.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="320.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="30" G="144" B="255" />
                    <PrePosition X="0.5333" Y="0.5000" />
                    <PreSize X="0.6667" Y="0.6667" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="202.9800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3383" />
                <PreSize X="0.8571" Y="0.1000" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_text_background_blue.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.6836" Y="0.7813" />
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