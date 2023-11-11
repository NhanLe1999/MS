<GameFile>
  <PropertyGroup Name="APAccountConnectionScreen" Type="Layer" ID="70572468-8436-424f-a39b-4640b3e419a1" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APAccountConnectionScreen" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" Visible="False" ActionTag="-933352803" Tag="207" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="2000.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.9531" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="white_background" ActionTag="-1817342593" Tag="579" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="1896228848" Tag="580" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="128.5000" RightMargin="128.5000" TopMargin="38.5000" BottomMargin="619.5000" FontSize="45" LabelText="Kết nối với tài khoản đã sử dụng&#xA;để giữ nguyên lộ trình học." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="767.0000" Y="110.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="674.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="44" G="184" B="233" />
                <PrePosition X="0.5000" Y="0.8783" />
                <PreSize X="0.7490" Y="0.1432" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="0" G="119" B="182" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="connection_button" ActionTag="258480769" CallBackType="Click" CallBackName="onConnectionClicked" Tag="581" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="192.0000" RightMargin="192.0000" TopMargin="478.0000" BottomMargin="202.0000" TouchEnable="True" FontSize="30" ButtonText="Kết nối" Scale9Enable="True" LeftEage="28" RightEage="28" TopEage="11" BottomEage="11" Scale9OriginX="28" Scale9OriginY="11" Scale9Width="3" Scale9Height="66" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="640.0000" Y="88.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="246.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3203" />
                <PreSize X="0.6250" Y="0.1146" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Normal" Path="account/new_v4/connection_button.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v4/connection_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="textfield_phone_bg" ActionTag="1396814302" Tag="1814" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="212.0000" RightMargin="212.0000" TopMargin="259.5000" BottomMargin="439.5000" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                <Size X="600.0000" Y="69.0000" />
                <Children>
                  <AbstractNodeData Name="textfield_enable" ActionTag="-359100576" Tag="583" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                    <Size X="600.0000" Y="69.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="account/new_v2/textfied_enable2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield_phone" ActionTag="1561297974" Tag="1816" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="15.6000" RightMargin="8.4000" TopMargin="6.9000" BottomMargin="6.9000" TouchEnable="True" FontSize="25" IsCustomSize="True" LabelText="" PlaceHolderText="05158455211236" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="576.0000" Y="55.2000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="591.6000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9860" Y="0.5000" />
                    <PreSize X="0.9600" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="474.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6172" />
                <PreSize X="0.5859" Y="0.0898" />
                <FileData Type="Normal" Path="account/new_v2/textfied_disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="textfield_password_bg" ActionTag="601778425" Tag="585" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="212.0000" RightMargin="212.0000" TopMargin="372.5000" BottomMargin="326.5000" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                <Size X="600.0000" Y="69.0000" />
                <Children>
                  <AbstractNodeData Name="textfield_enable" ActionTag="1083176842" Tag="586" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                    <Size X="600.0000" Y="69.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="account/new_v2/textfied_enable2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield_password" ActionTag="-166608169" Tag="587" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="30.0000" RightMargin="30.0000" TopMargin="6.9000" BottomMargin="6.9000" TouchEnable="True" FontSize="25" IsCustomSize="True" LabelText="" PlaceHolderText="enter your password" MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="540.0000" Y="55.2000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="300.0000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9000" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="show_password_btn" ActionTag="1710631668" CallBackType="Click" CallBackName="onShowPasswordButtonClicked" Tag="588" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="500.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="69.0000" />
                    <Children>
                      <AbstractNodeData Name="eye" ActionTag="-1419037039" Tag="589" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="26.0000" BottomMargin="26.0000" LeftEage="9" RightEage="9" TopEage="5" BottomEage="5" Scale9OriginX="9" Scale9OriginY="5" Scale9Width="10" Scale9Height="7" ctype="ImageViewObjectData">
                        <Size X="28.0000" Y="17.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="34.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2800" Y="0.2464" />
                        <FileData Type="Normal" Path="account/new_v2/eye_enable.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="600.0000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.1667" Y="1.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="361.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4701" />
                <PreSize X="0.5859" Y="0.0898" />
                <FileData Type="Normal" Path="account/new_v2/textfied_disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="small_title" ActionTag="-1199109373" Tag="626" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="233.0000" RightMargin="233.0000" TopMargin="182.5000" BottomMargin="542.5000" FontSize="35" LabelText="Thông tin tài khoản đã sử dụng" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="558.0000" Y="43.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="564.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.7344" />
                <PreSize X="0.5449" Y="0.0560" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="0" G="119" B="182" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_3" ActionTag="-1365746282" Tag="452" IconVisible="False" PositionPercentXEnabled="True" TopMargin="568.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="apple_button" ActionTag="-32311484" CallBackType="Click" CallBackName="onAppleButtonClicked" Tag="85" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="323.0528" RightMargin="636.9472" TopMargin="81.6737" BottomMargin="54.3263" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="34" Scale9Height="42" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="64.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.4803" ScaleY="0.3674" />
                    <Position X="353.7920" Y="77.8399" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3455" Y="0.3892" />
                    <PreSize X="0.0625" Y="0.3200" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="account/new_v2/apple_button.png" Plist="" />
                    <NormalFileData Type="Normal" Path="account/new_v2/apple_button.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="fb_button" ActionTag="-421741989" CallBackType="Click" CallBackName="onFacebookButtonClicked" Tag="660" IconVisible="False" LeftMargin="485.2120" RightMargin="473.7880" TopMargin="81.1692" BottomMargin="53.8308" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="35" Scale9Height="43" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="65.0000" Y="65.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="517.7120" Y="86.3308" />
                    <Scale ScaleX="1.2000" ScaleY="1.2000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5056" Y="0.4317" />
                    <PreSize X="0.0635" Y="0.3250" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="account/new_v2/fb_button.png" Plist="" />
                    <NormalFileData Type="Normal" Path="account/new_v2/fb_button.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="gmail_button" ActionTag="-1280187526" CallBackType="Click" CallBackName="onGmailButtonClicked" Tag="661" IconVisible="False" LeftMargin="654.9117" RightMargin="304.0883" TopMargin="80.6714" BottomMargin="53.3286" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="35" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="65.0000" Y="66.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="687.4117" Y="86.3286" />
                    <Scale ScaleX="1.2000" ScaleY="1.2000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6713" Y="0.4316" />
                    <PreSize X="0.0635" Y="0.3300" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="account/new_v2/gmail_button.png" Plist="" />
                    <NormalFileData Type="Normal" Path="account/new_v2/gmail_button.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="100.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1302" />
                <PreSize X="1.0000" Y="0.2604" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="or_txt" ActionTag="2018229040" Tag="627" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="234.5000" RightMargin="234.5000" TopMargin="587.5000" BottomMargin="143.5000" FontSize="30" LabelText="- - - - - - - - - - - - - Hoặc - - - - - - - - - - - -" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="555.0000" Y="37.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="162.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.2109" />
                <PreSize X="0.5420" Y="0.0482" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="0" G="119" B="182" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_3_0" ActionTag="-371407447" Tag="302" IconVisible="False" PositionPercentXEnabled="True" TopMargin="568.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="fb_button" ActionTag="-461942241" CallBackType="Click" CallBackName="onFacebookButtonClicked" Tag="304" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="325.9000" RightMargin="633.1000" TopMargin="81.1700" BottomMargin="53.8300" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="35" Scale9Height="43" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="65.0000" Y="65.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="358.4000" Y="86.3300" />
                    <Scale ScaleX="1.2000" ScaleY="1.2000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3500" Y="0.4317" />
                    <PreSize X="0.0635" Y="0.3250" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="account/new_v2/fb_button.png" Plist="" />
                    <NormalFileData Type="Normal" Path="account/new_v2/fb_button.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="gmail_button" ActionTag="1742893984" CallBackType="Click" CallBackName="onGmailButtonClicked" Tag="305" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="633.1000" RightMargin="325.9000" TopMargin="80.6700" BottomMargin="53.3300" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="35" Scale9Height="44" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="65.0000" Y="66.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="665.6000" Y="86.3300" />
                    <Scale ScaleX="1.2000" ScaleY="1.2000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6500" Y="0.4317" />
                    <PreSize X="0.0635" Y="0.3300" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="account/new_v2/gmail_button.png" Plist="" />
                    <NormalFileData Type="Normal" Path="account/new_v2/gmail_button.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="100.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1302" />
                <PreSize X="1.0000" Y="0.2604" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="skip_button" ActionTag="776128673" CallBackType="Click" CallBackName="onSkipButton" Tag="694" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="830.3208" RightMargin="-33.3208" TopMargin="717.7172" BottomMargin="16.2828" TouchEnable="True" FontSize="25" ButtonText="Bỏ Qua" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="67" Scale9Height="20" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="227.0000" Y="34.0000" />
                <Children>
                  <AbstractNodeData Name="line" ActionTag="-790563239" CallBackType="Click" CallBackName="onLogin" Tag="695" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="61.2900" RightMargin="52.2100" TopMargin="9.5000" BottomMargin="-24.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="113.5000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="118.0400" />
                    <Scale ScaleX="1.0000" ScaleY="0.0500" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5200" />
                    <PreSize X="0.5000" Y="1.4412" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="943.8208" Y="33.2828" />
                <Scale ScaleX="0.8500" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9217" Y="0.0433" />
                <PreSize X="0.2217" Y="0.0443" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <TextColor A="255" R="0" G="0" B="0" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_back" ActionTag="-1583506299" CallBackType="Click" CallBackName="onCancel" Tag="55" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2000" RightMargin="923.8000" TopMargin="16.8176" BottomMargin="653.1824" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="68" Scale9Height="76" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="98.0000" Y="98.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="51.2000" Y="702.1824" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0500" Y="0.9143" />
                <PreSize X="0.0957" Y="0.1276" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="account/new_v3/button_back_left.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v3/button_back_left.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>