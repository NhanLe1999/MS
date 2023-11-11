<GameFile>
  <PropertyGroup Name="APChangeDefaultPasswordScreen" Type="Layer" ID="b222d4e4-ea85-4f22-ae4f-770087cbffba" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APChangeDefaultPasswordScreen" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="white_background" ActionTag="-933352803" Tag="207" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="background" ActionTag="-1817342593" Tag="579" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="1896228848" Tag="580" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="351.0000" RightMargin="351.0000" TopMargin="66.0000" BottomMargin="647.0000" FontSize="45" LabelText="Đổi mật khẩu" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="322.0000" Y="55.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="674.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="44" G="184" B="233" />
                <PrePosition X="0.5000" Y="0.8783" />
                <PreSize X="0.3145" Y="0.0716" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="0" G="119" B="182" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="connection_button" ActionTag="258480769" CallBackType="Click" CallBackName="onConnectionClicked" Tag="581" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="172.0000" RightMargin="172.0000" TopMargin="516.9997" BottomMargin="161.0003" TouchEnable="True" FontSize="30" ButtonText="Kết nối" Scale9Enable="True" LeftEage="28" RightEage="28" TopEage="11" BottomEage="11" Scale9OriginX="28" Scale9OriginY="11" Scale9Width="3" Scale9Height="66" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="680.0000" Y="90.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="206.0003" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2682" />
                <PreSize X="0.6641" Y="0.1172" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Normal" Path="account/new_v4/connection_button.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v4/connection_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="textfield_password_bg" ActionTag="-1107956647" Tag="1244" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="187.0000" RightMargin="187.0000" TopMargin="287.0000" BottomMargin="401.0000" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                <Size X="650.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="textfield_enable" ActionTag="2144545479" Tag="1245" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                    <Size X="650.0000" Y="80.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="325.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="account/new_v2/textfied_enable2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield_password" ActionTag="1832828459" Tag="1246" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="32.5000" RightMargin="32.5000" TopMargin="8.0000" BottomMargin="8.0000" TouchEnable="True" FontSize="25" IsCustomSize="True" LabelText="" PlaceHolderText="enter your password" MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="585.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="325.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9000" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="show_password_btn" ActionTag="-570569932" CallBackType="Click" CallBackName="onShowPasswordButtonClicked" Tag="999" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="550.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="80.0000" />
                    <Children>
                      <AbstractNodeData Name="eye" ActionTag="1413999118" Tag="1248" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="31.5000" BottomMargin="31.5000" LeftEage="9" RightEage="9" TopEage="5" BottomEage="5" Scale9OriginX="9" Scale9OriginY="5" Scale9Width="10" Scale9Height="7" ctype="ImageViewObjectData">
                        <Size X="28.0000" Y="17.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2800" Y="0.2125" />
                        <FileData Type="Normal" Path="account/new_v2/eye_enable.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="650.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.1538" Y="1.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="441.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5742" />
                <PreSize X="0.6348" Y="0.1042" />
                <FileData Type="Normal" Path="account/new_v2/textfied_disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="textfield_repassword_bg" ActionTag="601778425" Tag="585" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="187.0000" RightMargin="187.0000" TopMargin="407.0000" BottomMargin="281.0000" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                <Size X="650.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="textfield_enable" ActionTag="1083176842" Tag="586" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="30" RightEage="30" TopEage="22" BottomEage="22" Scale9OriginX="30" Scale9OriginY="22" Scale9Width="12" Scale9Height="25" ctype="ImageViewObjectData">
                    <Size X="650.0000" Y="80.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="325.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="account/new_v2/textfied_enable2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="textfield_repassword" ActionTag="-166608169" Tag="587" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="32.5000" RightMargin="32.5000" TopMargin="8.0000" BottomMargin="8.0000" TouchEnable="True" FontSize="25" IsCustomSize="True" LabelText="" PlaceHolderText="enter your password" MaxLengthText="10" PasswordEnable="True" ctype="TextFieldObjectData">
                    <Size X="585.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="325.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9000" Y="0.8000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="show_password_btn" ActionTag="1710631668" CallBackType="Click" CallBackName="onShowPasswordButtonClicked" Tag="588" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="550.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="80.0000" />
                    <Children>
                      <AbstractNodeData Name="eye" ActionTag="-1419037039" Tag="589" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="36.0000" TopMargin="31.5000" BottomMargin="31.5000" LeftEage="9" RightEage="9" TopEage="5" BottomEage="5" Scale9OriginX="9" Scale9OriginY="5" Scale9Width="10" Scale9Height="7" ctype="ImageViewObjectData">
                        <Size X="28.0000" Y="17.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2800" Y="0.2125" />
                        <FileData Type="Normal" Path="account/new_v2/eye_enable.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="650.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="0.5000" />
                    <PreSize X="0.1538" Y="1.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="321.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4180" />
                <PreSize X="0.6348" Y="0.1042" />
                <FileData Type="Normal" Path="account/new_v2/textfied_disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="small_title" ActionTag="-1199109373" Tag="626" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="33.5000" RightMargin="33.5000" TopMargin="163.0002" BottomMargin="518.9998" FontSize="35" LabelText="Bạn đang dùng mật khẩu mặc định. Vui lòng thay đổi&#xA;mật khẩu để tăng cường bảo mật và dễ nhớ hơn." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="957.0000" Y="86.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="561.9998" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.7318" />
                <PreSize X="0.9346" Y="0.1120" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                <OutlineColor A="255" R="0" G="119" B="182" />
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