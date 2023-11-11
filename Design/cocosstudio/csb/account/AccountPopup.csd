<GameFile>
  <PropertyGroup Name="AccountPopup" Type="Layer" ID="6c7bdbfb-8a25-4c9b-a4a1-a629092318cd" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="AccountPopup" Tag="29" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="root_layout" ActionTag="1256518653" Tag="30" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
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
          <AbstractNodeData Name="bg" ActionTag="2070121388" Tag="31" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="212.0000" RightMargin="212.0000" TopMargin="184.0000" BottomMargin="184.0000" Scale9Enable="True" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
            <Size X="600.0000" Y="400.0000" />
            <Children>
              <AbstractNodeData Name="close" ActionTag="2128953405" VisibleForFrame="False" CallBackType="Click" CallBackName="onClose" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="538.0000" RightMargin="-38.0000" TopMargin="-42.0000" BottomMargin="342.0000" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="12" BottomEage="12" Scale9OriginX="12" Scale9OriginY="12" Scale9Width="76" Scale9Height="76" ctype="ImageViewObjectData">
                <Size X="100.0000" Y="100.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="588.0000" Y="392.0000" />
                <Scale ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9800" Y="0.9800" />
                <PreSize X="0.1667" Y="0.2500" />
                <FileData Type="Normal" Path="mjstory/new_ui/button_story_menu_close.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="131515301" Tag="32" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="22.5000" RightMargin="22.5000" TopMargin="42.5000" BottomMargin="162.5000" IsCustomSize="True" FontSize="32" LabelText="Ba mẹ vui lòng đăng ký tài khoản để sử dụng tính năng này&#xA;" TouchScaleChangeAble="True" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="555.0000" Y="195.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="260.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.6500" />
                <PreSize X="0.9250" Y="0.4875" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_left" ActionTag="-347815835" CallBackType="Click" CallBackName="onLeft" Tag="33" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="85.0000" RightMargin="325.0000" TopMargin="286.5000" BottomMargin="46.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="127" Scale9Height="24" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="190.0000" Y="67.0000" />
                <Children>
                  <AbstractNodeData Name="text_left" ActionTag="1511114281" Tag="35" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.0000" RightMargin="19.0000" TopMargin="10.6600" BottomMargin="13.3400" FontSize="35" LabelText="Đăng ký" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="152.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="95.0000" Y="34.8400" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5200" />
                    <PreSize X="0.8000" Y="0.6418" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="180.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3000" Y="0.2000" />
                <PreSize X="0.3167" Y="0.1675" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="account/button_grey.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/button_grey.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_right" ActionTag="-31498669" Alpha="252" CallBackType="Click" CallBackName="onRight" Tag="36" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="325.0000" RightMargin="85.0000" TopMargin="286.5000" BottomMargin="46.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="127" Scale9Height="24" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="190.0000" Y="67.0000" />
                <Children>
                  <AbstractNodeData Name="text_right" ActionTag="1988595256" Tag="37" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="31.5000" RightMargin="31.5000" TopMargin="10.6600" BottomMargin="13.3400" FontSize="35" LabelText="Bỏ qua" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="127.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="95.0000" Y="34.8400" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5200" />
                    <PreSize X="0.6684" Y="0.6418" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="420.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7000" Y="0.2000" />
                <PreSize X="0.3167" Y="0.1675" />
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
            <PreSize X="0.5859" Y="0.5208" />
            <FileData Type="Normal" Path="mjstory/pay/pay_popup_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>