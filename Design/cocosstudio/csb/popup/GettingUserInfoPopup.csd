<GameFile>
  <PropertyGroup Name="GettingUserInfoPopup" Type="Layer" ID="358b9f91-72ae-4489-97b7-cb65be7b823c" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="GettingUserInfoPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-207189224" Tag="1789" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="187.0002" RightMargin="186.9998" TopMargin="92.7353" BottomMargin="95.2647" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="10" BottomEage="10" Scale9OriginX="15" Scale9OriginY="10" Scale9Width="64" Scale9Height="62" ctype="ImageViewObjectData">
            <Size X="650.0000" Y="580.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="-1396093896" CallBackType="Click" CallBackName="onCancel" Tag="2137" IconVisible="False" LeftMargin="611.8513" RightMargin="-21.8513" TopMargin="-20.3507" BottomMargin="540.3507" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
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
                <Position X="641.8513" Y="570.3507" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9875" Y="0.9834" />
                <PreSize X="0.0923" Y="0.1034" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1499723513" Tag="1791" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="18.9990" BottomMargin="461.0009" IsCustomSize="True" FontSize="26" LabelText="Monkey gởi tặng ba mẹ Ebook &quot;Học toán tiếng anh hiệu quả cùng Monkey Math&quot;  ba mẹ vui lòng cung cấp thông tin để nhận Ebook" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="600.0000" Y="100.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="511.0009" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8810" />
                <PreSize X="0.9231" Y="0.1724" />
                <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="name_textfield_bg" ActionTag="-2132174653" Tag="1792" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="75.0028" RightMargin="74.9972" TopMargin="165.8495" BottomMargin="349.1505" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="500.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="name_txt" ActionTag="1808548331" Tag="446" IconVisible="False" LeftMargin="-0.0301" RightMargin="455.0301" TopMargin="-33.5501" BottomMargin="69.5501" FontSize="25" LabelText="Tên" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="45.0000" Y="29.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="-0.0301" Y="84.0501" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="-0.0001" Y="1.2931" />
                    <PreSize X="0.0900" Y="0.4462" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="name_textfield" ActionTag="-833830603" CallBackType="Event" Tag="1793" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="10.0000" RightMargin="10.0000" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="32" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="480.0000" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0028" Y="381.6505" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6580" />
                <PreSize X="0.7692" Y="0.1121" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="email_textfield_bg" ActionTag="-14858678" Tag="453" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="75.0000" RightMargin="75.0000" TopMargin="277.1967" BottomMargin="237.8033" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="500.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="email_txt" ActionTag="-387542719" Tag="454" IconVisible="False" LeftMargin="-0.4084" RightMargin="435.4084" TopMargin="-32.5502" BottomMargin="68.5502" FontSize="25" LabelText="Email" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="65.0000" Y="29.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="-0.4084" Y="83.0502" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="-0.0008" Y="1.2777" />
                    <PreSize X="0.1300" Y="0.4462" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="email_textfield" ActionTag="634488650" CallBackType="Event" Tag="455" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="10.0000" RightMargin="10.0000" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="32" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="480.0000" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="270.3033" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4660" />
                <PreSize X="0.7692" Y="0.1121" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="phone_textfield_bg" ActionTag="-551521628" Tag="456" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="75.0000" RightMargin="75.0000" TopMargin="391.6155" BottomMargin="123.3845" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="500.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="phone_txt" ActionTag="-2036476960" Tag="457" IconVisible="False" LeftMargin="-0.0301" RightMargin="350.0301" TopMargin="-33.5500" BottomMargin="69.5500" FontSize="25" LabelText="Số điện thoai" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="150.0000" Y="29.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="-0.0301" Y="84.0500" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="-0.0001" Y="1.2931" />
                    <PreSize X="0.3000" Y="0.4462" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="phone_textfield" ActionTag="-565982558" CallBackType="Event" Tag="458" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="10.0000" RightMargin="10.0000" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="32" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="480.0000" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="250.0000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="155.8845" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2688" />
                <PreSize X="0.7692" Y="0.1121" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="finish_button" ActionTag="-729483168" CallBackType="Click" CallBackName="onFinishButtonClicked" Tag="1798" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="73.7072" RightMargin="376.2928" TopMargin="485.2159" BottomMargin="44.7841" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="11" BottomEage="11" Scale9OriginX="13" Scale9OriginY="11" Scale9Width="131" Scale9Height="24" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="50.0000" />
                <Children>
                  <AbstractNodeData Name="finish_button_title" ActionTag="-1301181576" CallBackName="onFinish" Tag="1799" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="31.5000" RightMargin="31.5000" TopMargin="10.5000" BottomMargin="10.5000" FontSize="25" LabelText="Hoàn Thành" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="137.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="25.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.6850" Y="0.5800" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="173.7072" Y="69.7841" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2672" Y="0.1203" />
                <PreSize X="0.3077" Y="0.0862" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="warning_label" ActionTag="846120600" VisibleForFrame="False" CallBackName="onFinish" Tag="1677" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="237.0000" RightMargin="237.0000" TopMargin="546.6129" BottomMargin="11.3872" FontSize="18" LabelText="(*) is required fields" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="176.0000" Y="22.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="325.0000" Y="22.3872" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="165" B="0" />
                <PrePosition X="0.5000" Y="0.0386" />
                <PreSize X="0.2708" Y="0.0379" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="skip_button" ActionTag="-2090029618" CallBackType="Click" CallBackName="onSkipButtonClicked" Tag="575" IconVisible="False" LeftMargin="459.1694" RightMargin="40.8306" TopMargin="499.7200" BottomMargin="41.2800" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="150.0000" Y="39.0000" />
                <Children>
                  <AbstractNodeData Name="line" ActionTag="-371743468" Tag="576" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="32.5000" BottomMargin="3.5000" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="11" BottomEage="11" Scale9OriginX="13" Scale9OriginY="11" Scale9Width="14" Scale9Height="14" ctype="ImageViewObjectData">
                    <Size X="100.0000" Y="3.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="75.0000" Y="5.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="191" G="191" B="191" />
                    <PrePosition X="0.5000" Y="0.1282" />
                    <PreSize X="0.6667" Y="0.0769" />
                    <FileData Type="Normal" Path="rating/button_bg.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="skip_title" ActionTag="1451003324" CallBackName="onFinish" Tag="574" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="33.5000" RightMargin="33.5000" TopMargin="5.0000" BottomMargin="5.0000" FontSize="25" LabelText="Bỏ Qua" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="83.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="75.0000" Y="19.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="77" G="77" B="77" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5533" Y="0.7436" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="534.1694" Y="60.7800" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8218" Y="0.1048" />
                <PreSize X="0.2308" Y="0.0672" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0002" Y="385.2647" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5016" />
            <PreSize X="0.6348" Y="0.7552" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>