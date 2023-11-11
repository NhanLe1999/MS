<GameFile>
  <PropertyGroup Name="APLoginPopup" Type="Layer" ID="d4529a7d-2a2d-40f2-be35-29af948152c5" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APLoginPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-394346387" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="235.5000" RightMargin="235.5000" TopMargin="146.5000" BottomMargin="146.5000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="64" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="553.0000" Y="475.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="1523184232" CallBackType="Click" CallBackName="onCancel" Tag="2061" IconVisible="False" LeftMargin="500.4045" RightMargin="-7.4045" TopMargin="-7.1845" BottomMargin="422.1845" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-1848641559" Tag="3335" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="35.0000" Y="35.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="30.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5833" Y="0.5833" />
                    <FileData Type="Normal" Path="account/exit.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="530.4045" Y="452.1845" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9591" Y="0.9520" />
                <PreSize X="0.1085" Y="0.1263" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1717790470" Tag="362" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="43.0018" BottomMargin="363.9982" FontSize="28" LabelText="Ba mẹ vui lòng nhập SĐT để đăng&#xA;ký sử dụng chương trình.." HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="481.0000" Y="68.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="276.5000" Y="397.9982" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8379" />
                <PreSize X="0.8698" Y="0.1432" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="input_textfield_bg" ActionTag="629959836" Tag="590" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="56.0000" RightMargin="56.0000" TopMargin="148.5000" BottomMargin="261.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="input_textfield" ActionTag="387899855" CallBackType="Event" Tag="591" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="32" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="423.3600" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="220.5000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="276.5000" Y="294.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6189" />
                <PreSize X="0.7975" Y="0.1368" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="password_textfield_bg" ActionTag="1314826383" Tag="461" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="56.0000" RightMargin="56.0000" TopMargin="245.5000" BottomMargin="164.5000" Scale9Enable="True" LeftEage="11" RightEage="11" TopEage="7" BottomEage="7" Scale9OriginX="11" Scale9OriginY="7" Scale9Width="157" Scale9Height="70" ctype="ImageViewObjectData">
                <Size X="441.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="password_textfield" ActionTag="2075043200" CallBackType="Event" Tag="462" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="8.8200" RightMargin="8.8200" TopMargin="12.4995" BottomMargin="0.5005" TouchEnable="True" FontSize="29" IsCustomSize="True" LabelText="" PlaceHolderText="Nhập mật khẩu ..." MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="423.3600" Y="52.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="220.5000" Y="26.5005" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="160" G="159" B="159" />
                    <PrePosition X="0.5000" Y="0.4077" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="276.5000" Y="197.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4147" />
                <PreSize X="0.7975" Y="0.1368" />
                <FileData Type="Normal" Path="account/grey_textbox_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="login" ActionTag="-1114202442" CallBackType="Click" CallBackName="onLogin" Tag="647" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="178.5000" RightMargin="178.5000" TopMargin="345.0000" BottomMargin="81.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="196.0000" Y="49.0000" />
                <Children>
                  <AbstractNodeData Name="login_button_title" ActionTag="714587029" Tag="648" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="88.0000" RightMargin="88.0000" TopMargin="9.5000" BottomMargin="9.5000" FontSize="25" LabelText="..." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="20.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="98.0000" Y="24.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1020" Y="0.6122" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="276.5000" Y="105.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2221" />
                <PreSize X="0.3544" Y="0.1032" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="forget_pasword_button" ActionTag="2077200477" CallBackType="Click" CallBackName="onForgetPasswordClicked" Tag="907" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="163.0000" RightMargin="163.0000" TopMargin="421.8901" BottomMargin="19.1099" TouchEnable="True" FontSize="28" ButtonText="Quên Mật Khẩu" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="197" Scale9Height="24" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="227.0000" Y="34.0000" />
                <Children>
                  <AbstractNodeData Name="line" ActionTag="-914543180" CallBackType="Click" CallBackName="onLogin" Tag="830" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.0400" RightMargin="10.9600" TopMargin="9.5000" BottomMargin="-24.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="196.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="118.0400" />
                    <Scale ScaleX="1.2000" ScaleY="0.0500" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5200" />
                    <PreSize X="0.8634" Y="1.4412" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="276.5000" Y="36.1099" />
                <Scale ScaleX="0.8500" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0760" />
                <PreSize X="0.4105" Y="0.0716" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <TextColor A="255" R="36" G="172" B="222" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.1000" ScaleY="1.1000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.5400" Y="0.6185" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>