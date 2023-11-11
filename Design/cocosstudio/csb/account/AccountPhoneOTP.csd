<GameFile>
  <PropertyGroup Name="AccountPhoneOTP" Type="Layer" ID="45ef3c25-dc49-4f96-835b-40e636103b0d" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APPhoneOTP" Tag="96" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="-1431561531" VisibleForFrame="False" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftEage="462" RightEage="462" TopEage="258" BottomEage="258" Scale9OriginX="-416" Scale9OriginY="-212" Scale9Width="878" Scale9Height="470" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="Panel_black" ActionTag="-323497629" Tag="111" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg_ngang" ActionTag="854990809" Tag="97" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="162.0000" RightMargin="162.0000" TopMargin="97.3872" BottomMargin="100.6128" LeftEage="204" RightEage="204" TopEage="183" BottomEage="183" Scale9OriginX="204" Scale9OriginY="183" Scale9Width="266" Scale9Height="95" ctype="ImageViewObjectData">
            <Size X="700.0000" Y="570.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="1719452963" Tag="98" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="78.0000" RightMargin="78.0000" TopMargin="34.3000" BottomMargin="444.7000" IsCustomSize="True" FontSize="30" LabelText="Ba mẹ vui lòng nhập mã đã nhận được qua SMS" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="544.0000" Y="91.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="490.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8600" />
                <PreSize X="0.7771" Y="0.1596" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="phone_number" ActionTag="-1244035677" Tag="611" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="235.5000" RightMargin="235.5000" TopMargin="145.8000" BottomMargin="385.2000" FontSize="32" LabelText="+84326217414" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="229.0000" Y="39.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="404.7000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="9" G="177" B="212" />
                <PrePosition X="0.5000" Y="0.7100" />
                <PreSize X="0.3271" Y="0.0684" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_phone" ActionTag="1189109274" CallBackName="onTrigger" Tag="648" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="145.0000" RightMargin="145.0000" TopMargin="250.0730" BottomMargin="239.9270" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="410.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Image_1" ActionTag="178900405" Tag="649" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="11.2500" RightMargin="318.7500" Scale9Enable="True" LeftEage="23" RightEage="23" TopEage="26" BottomEage="26" Scale9OriginX="23" Scale9OriginY="26" Scale9Width="25" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="80.0000" />
                    <Children>
                      <AbstractNodeData Name="Text" ActionTag="-96362947" Tag="651" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="50" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="80.0000" Y="80.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="51.2500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1250" Y="0.5000" />
                    <PreSize X="0.1951" Y="1.0000" />
                    <FileData Type="Normal" Path="account/intro_license_letter.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="1513864082" Tag="652" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="113.7500" RightMargin="216.2500" Scale9Enable="True" LeftEage="23" RightEage="23" TopEage="26" BottomEage="26" Scale9OriginX="23" Scale9OriginY="26" Scale9Width="25" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="80.0000" />
                    <Children>
                      <AbstractNodeData Name="Text" ActionTag="946975788" Tag="653" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="50" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="80.0000" Y="80.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="153.7500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3750" Y="0.5000" />
                    <PreSize X="0.1951" Y="1.0000" />
                    <FileData Type="Normal" Path="account/intro_license_letter.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_3" ActionTag="-247841590" Tag="654" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="216.2500" RightMargin="113.7500" Scale9Enable="True" LeftEage="23" RightEage="23" TopEage="26" BottomEage="26" Scale9OriginX="23" Scale9OriginY="26" Scale9Width="25" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="80.0000" />
                    <Children>
                      <AbstractNodeData Name="Text" ActionTag="-1495491769" Tag="655" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="50" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="80.0000" Y="80.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="256.2500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6250" Y="0.5000" />
                    <PreSize X="0.1951" Y="1.0000" />
                    <FileData Type="Normal" Path="account/intro_license_letter.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_4" ActionTag="1514481435" Tag="656" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="318.7500" RightMargin="11.2500" Scale9Enable="True" LeftEage="23" RightEage="23" TopEage="26" BottomEage="26" Scale9OriginX="23" Scale9OriginY="26" Scale9Width="25" Scale9Height="21" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="80.0000" />
                    <Children>
                      <AbstractNodeData Name="Text" ActionTag="-1531347197" Tag="657" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" IsCustomSize="True" FontSize="50" LabelText="" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="80.0000" Y="80.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.0000" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="358.7500" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8750" Y="0.5000" />
                    <PreSize X="0.1951" Y="1.0000" />
                    <FileData Type="Normal" Path="account/intro_license_letter.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="icon_warning" ActionTag="-952412819" VisibleForFrame="False" Tag="140" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="32.0000" RightMargin="360.0000" TopMargin="92.8000" BottomMargin="-28.8000" LeftEage="5" RightEage="5" TopEage="5" BottomEage="5" Scale9OriginX="5" Scale9OriginY="5" Scale9Width="8" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="18.0000" Y="16.0000" />
                    <Children>
                      <AbstractNodeData Name="warning_label" ActionTag="869889931" Tag="141" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="27.3672" RightMargin="-324.3672" TopMargin="-1.7856" BottomMargin="-12.2144" IsCustomSize="True" FontSize="18" LabelText="Vui lòng nhập thông tin vào ô trống" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="315.0000" Y="30.0000" />
                        <AnchorPoint ScaleY="1.0000" />
                        <Position X="27.3672" Y="17.7856" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="254" G="2" B="1" />
                        <PrePosition X="1.5204" Y="1.1116" />
                        <PreSize X="17.5000" Y="1.8750" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="41.0000" Y="-20.8000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1000" Y="-0.2600" />
                    <PreSize X="0.0439" Y="0.2000" />
                    <FileData Type="Normal" Path="license/active_license/activelicense_iconwarning.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="279.9270" />
                <Scale ScaleX="1.2500" ScaleY="1.2500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4911" />
                <PreSize X="0.5857" Y="0.1404" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="layout_name" ActionTag="-994286533" CallBackName="onTrigger" Tag="99" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="145.0000" RightMargin="145.0000" TopMargin="245.0000" BottomMargin="245.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="410.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="test" ActionTag="1040690727" VisibleForFrame="False" Tag="367" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="10.0000" BottomMargin="10.0000" Scale9Enable="True" LeftEage="23" RightEage="23" TopEage="26" BottomEage="26" Scale9OriginX="23" Scale9OriginY="20" Scale9Width="1" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="410.0000" Y="60.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="205.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="0.7500" />
                    <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
                  </AbstractNodeData>
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
                <Position X="350.0000" Y="285.0000" />
                <Scale ScaleX="1.2500" ScaleY="1.2500" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.5857" Y="0.1404" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_next" ActionTag="1367600596" CallBackType="Click" CallBackName="onNext" Tag="102" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="105.0000" RightMargin="385.0000" TopMargin="443.1000" BottomMargin="66.9000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="127" Scale9Height="16" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="210.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="text_next" ActionTag="-1488165383" Tag="103" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="30.5000" RightMargin="30.5000" TopMargin="8.5000" BottomMargin="8.5000" FontSize="35" LabelText="Tiếp tục" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="149.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="105.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7095" Y="0.7167" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="210.0000" Y="96.9000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3000" Y="0.1700" />
                <PreSize X="0.3000" Y="0.1053" />
                <TextColor A="255" R="255" G="255" B="255" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_resend" ActionTag="-2039807652" CallBackType="Click" CallBackName="onResend" Tag="636" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="385.0000" RightMargin="105.0000" TopMargin="443.1000" BottomMargin="66.9000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="127" Scale9Height="16" DisplayState="False" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="210.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="text_resend" ActionTag="-2043515437" Tag="637" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="9.5000" RightMargin="9.5000" TopMargin="8.5000" BottomMargin="8.5000" FontSize="35" LabelText="Gửi lại (30)" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="191.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="105.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.9095" Y="0.7167" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="490.0000" Y="96.9000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7000" Y="0.1700" />
                <PreSize X="0.3000" Y="0.1053" />
                <TextColor A="255" R="255" G="255" B="255" />
                <PressedFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/login/button seleced.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="close" ActionTag="1447806189" CallBackType="Click" CallBackName="onClose" Tag="104" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="660.0001" RightMargin="-12.0001" TopMargin="-14.5999" BottomMargin="532.5999" TouchEnable="True" LeftEage="17" RightEage="17" TopEage="17" BottomEage="17" Scale9OriginX="17" Scale9OriginY="17" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                <Size X="52.0000" Y="52.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="686.0001" Y="558.5999" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9800" Y="0.9800" />
                <PreSize X="0.0743" Y="0.0912" />
                <FileData Type="Normal" Path="group_promo_close.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="385.6128" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5021" />
            <PreSize X="0.6836" Y="0.7422" />
            <FileData Type="Normal" Path="license/active_license/activelicense_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>