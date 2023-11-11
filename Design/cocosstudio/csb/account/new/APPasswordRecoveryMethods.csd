<GameFile>
  <PropertyGroup Name="APPasswordRecoveryMethods" Type="Layer" ID="73a9b864-5a74-464f-b1a1-9a20bbe166e1" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APPasswordRecoveryMethods" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-394346387" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="102.0000" RightMargin="102.0000" TopMargin="54.0000" BottomMargin="54.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="64" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="820.0000" Y="660.0000" />
            <Children>
              <AbstractNodeData Name="exit_button" ActionTag="1601734562" CallBackType="Click" CallBackName="onCancel" Tag="2121" IconVisible="False" LeftMargin="762.4465" RightMargin="-2.4465" TopMargin="-3.7625" BottomMargin="603.7625" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="-1792120638" Tag="3381" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
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
                <Position X="792.4465" Y="633.7625" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9664" Y="0.9602" />
                <PreSize X="0.0732" Y="0.0909" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1717790470" Tag="362" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="82.5000" RightMargin="82.5000" TopMargin="45.0000" BottomMargin="545.0000" FontSize="29" LabelText=" Bạn muốn đặt lại mật khẩu bằng cách nào?&#xA;" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="655.0000" Y="70.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="410.0000" Y="580.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8788" />
                <PreSize X="0.7988" Y="0.1061" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="mail_cb" ActionTag="1130442279" Tag="1279" IconVisible="False" LeftMargin="41.0000" RightMargin="753.0000" TopMargin="158.0000" BottomMargin="475.0000" TouchEnable="True" CheckedState="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="mail_text" ActionTag="1439360178" Tag="1280" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="-285.0000" TopMargin="-4.0000" BottomMargin="-4.0000" FontSize="29" LabelText="Gửi mã qua email" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="261.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="50.0000" Y="13.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.9231" Y="0.5000" />
                    <PreSize X="10.0385" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="email_text" ActionTag="-1945773884" Tag="1281" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="-630.0000" TopMargin="40.1089" BottomMargin="-50.1089" FontSize="30" LabelText="nguyenhoangthienphuoc@gmail.com" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="606.0000" Y="37.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="50.0000" Y="-31.6089" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="36" G="172" B="222" />
                    <PrePosition X="1.9231" Y="-1.1707" />
                    <PreSize X="23.3077" Y="1.3704" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="54.0000" Y="488.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0659" Y="0.7402" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="sms_cb" ActionTag="-1153439456" Tag="1285" IconVisible="False" LeftMargin="41.0000" RightMargin="753.0000" TopMargin="283.0000" BottomMargin="350.0000" TouchEnable="True" CheckedState="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="sms_text" ActionTag="569906571" Tag="1286" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="-269.0000" TopMargin="-4.0000" BottomMargin="-4.0000" FontSize="29" LabelText="Gửi mã qua SMS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="245.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="50.0000" Y="13.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.9231" Y="0.5000" />
                    <PreSize X="9.4231" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="phone_number_text" ActionTag="1650736310" Tag="1287" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="-218.0000" TopMargin="40.1089" BottomMargin="-50.1089" FontSize="30" LabelText="0909999999" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="194.0000" Y="37.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="50.0000" Y="-31.6089" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="36" G="172" B="222" />
                    <PrePosition X="1.9231" Y="-1.1707" />
                    <PreSize X="7.4615" Y="1.3704" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="54.0000" Y="363.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0659" Y="0.5508" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="question_cb" ActionTag="997891444" Tag="1288" IconVisible="False" LeftMargin="41.0000" RightMargin="753.0000" TopMargin="409.5000" BottomMargin="223.5000" TouchEnable="True" CheckedState="True" ctype="CheckBoxObjectData">
                <Size X="26.0000" Y="27.0000" />
                <Children>
                  <AbstractNodeData Name="question_text" ActionTag="334349743" Tag="1289" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="-550.0000" TopMargin="-4.0000" BottomMargin="-4.0000" FontSize="29" LabelText="Trả lời câu hỏi để xác thực tài khoản" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="526.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="50.0000" Y="13.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="1.9231" Y="0.5000" />
                    <PreSize X="20.2308" Y="1.2963" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="54.0000" Y="237.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0659" Y="0.3591" />
                <PreSize X="0.0317" Y="0.0409" />
                <NormalBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <PressedBackFileData Type="Normal" Path="cb_checked.png" Plist="" />
                <DisableBackFileData Type="Normal" Path="cb_normal.png" Plist="" />
                <NodeNormalFileData Type="Default" Path="Default/CheckBoxNode_Normal.png" Plist="" />
                <NodeDisableFileData Type="Default" Path="Default/CheckBoxNode_Disable.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="continute_button" ActionTag="-235109591" CallBackType="Click" CallBackName="onContinute" Tag="1303" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="277.0000" RightMargin="277.0000" TopMargin="537.0000" BottomMargin="54.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="266.0000" Y="69.0000" />
                <Children>
                  <AbstractNodeData Name="continue_text_title" ActionTag="556828574" Tag="1304" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="79.5000" RightMargin="79.5000" TopMargin="19.5000" BottomMargin="19.5000" FontSize="25" LabelText="Tiếp tục" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="107.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="133.0000" Y="34.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4023" Y="0.4348" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="410.0000" Y="88.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1341" />
                <PreSize X="0.3244" Y="0.1045" />
                <TextColor A="255" R="255" G="255" B="255" />
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
            <PreSize X="0.8008" Y="0.8594" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>