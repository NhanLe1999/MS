<GameFile>
  <PropertyGroup Name="RewardPopup" Type="Layer" ID="c03aa3df-5658-4f88-8dce-e5889c7a8eec" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="RewardPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-207189224" Tag="1789" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="187.0000" RightMargin="187.0000" TopMargin="134.0000" BottomMargin="134.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="10" BottomEage="10" Scale9OriginX="15" Scale9OriginY="10" Scale9Width="64" Scale9Height="62" ctype="ImageViewObjectData">
            <Size X="650.0000" Y="500.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="-1396093896" CallBackType="Click" CallBackName="onCancel" Tag="2137" IconVisible="False" LeftMargin="600.4700" RightMargin="-10.4700" TopMargin="-6.7683" BottomMargin="446.7683" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="1451393185" Tag="3397" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="6.5000" RightMargin="6.5000" TopMargin="6.5000" BottomMargin="6.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="33" Scale9Height="33" ctype="ImageViewObjectData">
                    <Size X="47.0000" Y="47.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="30.0000" Y="30.0000" />
                    <Scale ScaleX="0.8000" ScaleY="0.8000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7833" Y="0.7833" />
                    <FileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="630.4700" Y="476.7683" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9700" Y="0.9535" />
                <PreSize X="0.0923" Y="0.1200" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="logo" ActionTag="1234237805" Tag="598" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-24.5000" RightMargin="-24.5000" TopMargin="-85.0496" BottomMargin="122.0496" LeftEage="230" RightEage="230" TopEage="152" BottomEage="152" Scale9OriginX="230" Scale9OriginY="152" Scale9Width="239" Scale9Height="159" ctype="ImageViewObjectData">
                <Size X="699.0000" Y="463.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="353.5496" />
                <Scale ScaleX="0.7500" ScaleY="0.7500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.7071" />
                <PreSize X="1.0754" Y="0.9260" />
                <FileData Type="Normal" Path="mjstory/monkey_logo_vi.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1499723513" Tag="1791" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="223.0811" BottomMargin="126.9189" IsCustomSize="True" FontSize="26" LabelText="Monkey gởi tặng ba mẹ Ebook &quot;Học toán tiếng anh hiệu quả cùng Monkey Math&quot;  ba mẹ vui lòng cung cấp thông tin để nhận Ebook" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="600.0000" Y="150.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="201.9189" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.4038" />
                <PreSize X="0.9231" Y="0.3000" />
                <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="download_button" ActionTag="-1224163517" CallBackType="Click" CallBackName="onDownloadButtonClicked" Tag="620" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="125.5000" RightMargin="125.5000" TopMargin="385.0000" BottomMargin="35.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="11" BottomEage="11" Scale9OriginX="13" Scale9OriginY="11" Scale9Width="56" Scale9Height="42" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="399.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="download_title" ActionTag="694929123" CallBackName="onFinish" Tag="621" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="134.0000" RightMargin="134.0000" TopMargin="25.5000" BottomMargin="25.5000" FontSize="25" LabelText="Tải về ngay" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="131.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="199.5000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3283" Y="0.3625" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="download_icon" ActionTag="-812367497" Tag="1083" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="14.5541" RightMargin="323.4459" TopMargin="13.0755" BottomMargin="5.9245" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="61.0000" Y="61.0000" />
                    <AnchorPoint ScaleX="0.5811" ScaleY="0.5586" />
                    <Position X="50.0000" Y="40.0000" />
                    <Scale ScaleX="0.8000" ScaleY="0.8000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1253" Y="0.5000" />
                    <PreSize X="0.1529" Y="0.7625" />
                    <FileData Type="Normal" Path="popup/download_icon.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="75.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1500" />
                <PreSize X="0.6138" Y="0.1600" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="popup/green_bg.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.6348" Y="0.6510" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>