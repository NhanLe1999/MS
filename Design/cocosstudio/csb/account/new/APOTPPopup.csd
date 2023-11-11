<GameFile>
  <PropertyGroup Name="APOTPPopup" Type="Layer" ID="18cb1ca7-0379-4ae7-bc08-7b3a1fff4043" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APOTPPopup" Tag="96" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-1116208871" Tag="1953" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="236.5000" RightMargin="236.5000" TopMargin="186.5000" BottomMargin="186.5000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="12" BottomEage="12" Scale9OriginX="15" Scale9OriginY="12" Scale9Width="64" Scale9Height="58" ctype="ImageViewObjectData">
            <Size X="551.0000" Y="395.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="1719452963" Tag="98" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="3.5000" RightMargin="3.5000" TopMargin="45.0000" BottomMargin="259.0000" IsCustomSize="True" FontSize="25" LabelText="Vui lòng kiểm tra tin nhắn trên điện thoại.&#xA;Chúng tôi đã gửi mã xác nhận đến số: " HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="544.0000" Y="91.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="304.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.7709" />
                <PreSize X="0.9873" Y="0.2304" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="exit_button" ActionTag="-863994120" CallBackType="Click" CallBackName="onCancel" Tag="2105" IconVisible="False" LeftMargin="498.4226" RightMargin="-7.4226" TopMargin="-5.6862" BottomMargin="340.6862" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="60.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="401841425" Tag="3365" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="21" Scale9Height="21" ctype="ImageViewObjectData">
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
                <Position X="528.4226" Y="370.6862" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9590" Y="0.9384" />
                <PreSize X="0.1089" Y="0.1519" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="input_text" ActionTag="-1244035677" Tag="611" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="29.0000" RightMargin="29.0000" TopMargin="109.9998" BottomMargin="255.0002" FontSize="25" LabelText="nguyenhoangthienphuoc@gmail.com" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="493.0000" Y="30.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="270.0002" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="9" G="177" B="212" />
                <PrePosition X="0.5000" Y="0.6835" />
                <PreSize X="0.8947" Y="0.0759" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_phone" ActionTag="1189109274" CallBackName="onTrigger" Tag="648" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="70.5000" RightMargin="70.5000" TopMargin="163.5000" BottomMargin="151.5000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="410.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="178900405" Tag="649" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="4.7500" RightMargin="312.2500" TopMargin="-3.5000" BottomMargin="-3.5000" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="14" Scale9Height="14" ctype="ImageViewObjectData">
                    <Size X="93.0000" Y="87.0000" />
                    <Children>
                      <AbstractNodeData Name="Text1" ActionTag="-96362947" Tag="651" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="67" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="93.0000" Y="87.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="46.5000" Y="43.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="51.2500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1250" Y="0.5000" />
                    <PreSize X="0.2268" Y="1.0875" />
                    <FileData Type="Normal" Path="account/grey9path.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="1513864082" Tag="652" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="107.2500" RightMargin="209.7500" TopMargin="-3.5000" BottomMargin="-3.5000" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="14" Scale9Height="14" ctype="ImageViewObjectData">
                    <Size X="93.0000" Y="87.0000" />
                    <Children>
                      <AbstractNodeData Name="Text2" ActionTag="946975788" Tag="653" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="67" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="93.0000" Y="87.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="46.5000" Y="43.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="153.7500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3750" Y="0.5000" />
                    <PreSize X="0.2268" Y="1.0875" />
                    <FileData Type="Normal" Path="account/grey9path.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_3" ActionTag="-247841590" Tag="654" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="209.7500" RightMargin="107.2500" TopMargin="-3.5000" BottomMargin="-3.5000" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="14" Scale9Height="14" ctype="ImageViewObjectData">
                    <Size X="93.0000" Y="87.0000" />
                    <Children>
                      <AbstractNodeData Name="Text3" ActionTag="-1495491769" Tag="655" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="67" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="93.0000" Y="87.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="46.5000" Y="43.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="256.2500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6250" Y="0.5000" />
                    <PreSize X="0.2268" Y="1.0875" />
                    <FileData Type="Normal" Path="account/grey9path.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_4" ActionTag="1514481435" Tag="656" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="312.2500" RightMargin="4.7500" TopMargin="-3.5000" BottomMargin="-3.5000" Scale9Enable="True" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="14" Scale9Height="14" ctype="ImageViewObjectData">
                    <Size X="93.0000" Y="87.0000" />
                    <Children>
                      <AbstractNodeData Name="Text4" ActionTag="-1531347197" Tag="657" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="67" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="93.0000" Y="87.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="46.5000" Y="43.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="358.7500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8750" Y="0.5000" />
                    <PreSize X="0.2268" Y="1.0875" />
                    <FileData Type="Normal" Path="account/grey9path.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="191.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4848" />
                <PreSize X="0.7441" Y="0.2025" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_name" ActionTag="-994286533" CallBackName="onTrigger" Tag="99" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="70.5000" RightMargin="70.5000" TopMargin="157.5000" BottomMargin="157.5000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="410.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="textfield" ActionTag="-429908862" Alpha="0" Tag="101" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FontSize="37" IsCustomSize="True" LabelText="" PlaceHolderText="" MaxLengthText="10" ctype="TextFieldObjectData">
                    <Size X="410.0000" Y="80.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="205.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="67" G="67" B="67" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="197.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.7441" Y="0.2025" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_continue" ActionTag="1367600596" CallBackType="Click" CallBackName="onContinute" Tag="102" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="177.5000" RightMargin="177.5000" TopMargin="265.0000" BottomMargin="81.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="127" Scale9Height="16" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="196.0000" Y="49.0000" />
                <Children>
                  <AbstractNodeData Name="continue_text_title" ActionTag="-1488165383" Tag="103" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="42.5000" RightMargin="42.5000" TopMargin="9.5000" BottomMargin="9.5000" FontSize="25" LabelText="Tiếp Tục" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="111.0000" Y="30.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="98.0000" Y="24.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5663" Y="0.6122" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="105.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2671" />
                <PreSize X="0.3557" Y="0.1241" />
                <TextColor A="255" R="255" G="255" B="255" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_resend" ActionTag="781897458" CallBackType="Click" CallBackName="onResend" Tag="1946" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="228.5000" RightMargin="228.5000" TopMargin="335.7751" BottomMargin="43.2249" TouchEnable="True" FontSize="25" ButtonText="Gửi lại mã (26)" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="151" Scale9Height="20" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="94.0000" Y="16.0000" />
                <Children>
                  <AbstractNodeData Name="line" ActionTag="702428678" VisibleForFrame="False" CallBackType="Click" CallBackName="onLogin" Tag="887" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="-51.0000" RightMargin="-51.0000" TopMargin="-3.5000" BottomMargin="-29.5000" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="113" Scale9Height="2" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="196.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.0000" Y="-5.0000" />
                    <Scale ScaleX="0.6000" ScaleY="0.0350" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="-0.3125" />
                    <PreSize X="2.0851" Y="3.0625" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="275.5000" Y="51.2249" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1297" />
                <PreSize X="0.1706" Y="0.0405" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="icon_warning" ActionTag="2065674490" VisibleForFrame="False" Tag="2100" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="178.3951" RightMargin="354.6049" TopMargin="279.5600" BottomMargin="99.4400" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="8" Scale9Height="6" ctype="ImageViewObjectData">
                <Size X="18.0000" Y="16.0000" />
                <Children>
                  <AbstractNodeData Name="warning_label" ActionTag="-758899810" Tag="2101" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="27.3672" RightMargin="-333.3672" TopMargin="-3.0000" BottomMargin="-3.0000" FontSize="18" LabelText="Vui lòng nhập thông tin vào ô trống" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="324.0000" Y="22.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="27.3672" Y="8.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="254" G="2" B="1" />
                    <PrePosition X="1.5204" Y="0.5000" />
                    <PreSize X="18.0000" Y="1.3750" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="187.3951" Y="107.4400" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3401" Y="0.2720" />
                <PreSize X="0.0327" Y="0.0405" />
                <FileData Type="Normal" Path="license/active_license/activelicense_iconwarning.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.1000" ScaleY="1.1000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.5381" Y="0.5143" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>