<GameFile>
  <PropertyGroup Name="LicenseLimitNotice" Type="Layer" ID="5d1adaaf-64f8-40d9-be23-3766b7ca9b42" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="30" Speed="1.0000">
        <Timeline ActionTag="-1305581361" Property="Alpha">
          <IntFrame FrameIndex="0" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="204">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Layer" CustomClassName="MSLicenseLimitPopup" Tag="184" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="-1305581361" Tag="193" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg" ActionTag="84601829" Tag="185" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="87.0000" RightMargin="87.0000" TopMargin="59.0000" BottomMargin="59.0000" Scale9Enable="True" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="1" Scale9Height="1" ctype="ImageViewObjectData">
            <Size X="850.0000" Y="650.0000" />
            <Children>
              <AbstractNodeData Name="parent.license.ask" ActionTag="572617541" Tag="188" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="50.0003" RightMargin="490.9997" TopMargin="29.9975" BottomMargin="593.0025" FontSize="22" LabelText="Thông báo về mã kích hoạt" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="309.0000" Y="27.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="50.0003" Y="620.0025" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.0588" Y="0.9539" />
                <PreSize X="0.3635" Y="0.0415" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button" ActionTag="1374439829" CallBackType="Click" CallBackName="onOK" Tag="191" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="375.0000" RightMargin="375.0000" TopMargin="589.9999" BottomMargin="15.0001" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="6" BottomEage="6" Scale9OriginX="6" Scale9OriginY="6" Scale9Width="8" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="100.0000" Y="45.0000" />
                <Children>
                  <AbstractNodeData Name="Text_2" ActionTag="73061829" Tag="192" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="29.5000" RightMargin="29.5000" TopMargin="8.0000" BottomMargin="8.0000" FontSize="24" LabelText="OK" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="41.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="22.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4100" Y="0.6444" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="425.0000" Y="37.5001" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0577" />
                <PreSize X="0.1176" Y="0.0692" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/parent/parent_notification_item_open_url.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.ask_0" ActionTag="-1728842422" Tag="214" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="51.0000" RightMargin="49.0000" TopMargin="59.9985" BottomMargin="510.0015" IsCustomSize="True" FontSize="22" LabelText="Bạn đã kích hoạt ứng dụng trên đủ 2 thiết bị. Để kích hoạt trên thiết bị này, vui lòng tắt kích hoạt ứng dụng trên 1 trong 2 thiết bị đã cài trước đây." ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="750.0000" Y="80.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="51.0000" Y="590.0015" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.0600" Y="0.9077" />
                <PreSize X="0.8824" Y="0.1231" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.ask_1" ActionTag="-157175654" Tag="215" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="50.0003" RightMargin="482.9997" TopMargin="159.9996" BottomMargin="463.0004" FontSize="22" LabelText="Hướng dẫn gỡ mã kích hoạt" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="317.0000" Y="27.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="50.0003" Y="490.0004" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.0588" Y="0.7538" />
                <PreSize X="0.3729" Y="0.0415" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_4" ActionTag="-395128076" Tag="231" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="BottomEdge" LeftMargin="25.0026" RightMargin="585.9974" TopMargin="245.9970" BottomMargin="107.0030" LeftEage="78" RightEage="78" TopEage="98" BottomEage="98" Scale9OriginX="78" Scale9OriginY="98" Scale9Width="83" Scale9Height="101" ctype="ImageViewObjectData">
                <Size X="239.0000" Y="297.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="144.5026" Y="255.5030" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1700" Y="0.3931" />
                <PreSize X="0.2812" Y="0.4569" />
                <FileData Type="Normal" Path="mjstory/new_ui/license_limit_step_1.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_4_0" ActionTag="-567072062" Tag="232" IconVisible="False" VerticalEdge="BottomEdge" LeftMargin="293.0015" RightMargin="317.9985" TopMargin="245.9970" BottomMargin="107.0030" LeftEage="78" RightEage="78" TopEage="98" BottomEage="98" Scale9OriginX="78" Scale9OriginY="98" Scale9Width="83" Scale9Height="101" ctype="ImageViewObjectData">
                <Size X="239.0000" Y="297.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="412.5015" Y="255.5030" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4853" Y="0.3931" />
                <PreSize X="0.2812" Y="0.4569" />
                <FileData Type="Normal" Path="mjstory/new_ui/license_limit_step_2.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_4_1" ActionTag="1302642773" Tag="233" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="BottomEdge" LeftMargin="560.9999" RightMargin="50.0001" TopMargin="245.9970" BottomMargin="107.0030" LeftEage="78" RightEage="78" TopEage="98" BottomEage="98" Scale9OriginX="78" Scale9OriginY="98" Scale9Width="83" Scale9Height="101" ctype="ImageViewObjectData">
                <Size X="239.0000" Y="297.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="680.4999" Y="255.5030" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8006" Y="0.3931" />
                <PreSize X="0.2812" Y="0.4569" />
                <FileData Type="Normal" Path="mjstory/new_ui/license_limit_step_3.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.ask_0_0" ActionTag="-996515205" Tag="234" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="50.0003" RightMargin="624.9997" TopMargin="547.9100" BottomMargin="80.0900" FontSize="18" LabelText="Chọn mục &quot;Bố mẹ&quot;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="175.0000" Y="22.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="50.0003" Y="102.0900" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.0588" Y="0.1571" />
                <PreSize X="0.2059" Y="0.0338" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.ask_0_0_0" ActionTag="1562967662" Tag="235" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="320.0029" RightMargin="297.9971" TopMargin="547.9100" BottomMargin="80.0900" FontSize="18" LabelText="Chọn mục &quot;Mã kích hoạt&quot;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="232.0000" Y="22.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="320.0029" Y="102.0900" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.3765" Y="0.1571" />
                <PreSize X="0.2729" Y="0.0338" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.ask_0_0_0_0" ActionTag="743880240" Tag="236" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="585.0028" RightMargin="29.9972" TopMargin="547.9098" BottomMargin="58.0902" FontSize="18" LabelText="Chọn mục &quot;Hủy kích hoạt&#xA;ứng dụng&quot;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="235.0000" Y="44.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="585.0028" Y="102.0902" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.6882" Y="0.1571" />
                <PreSize X="0.2765" Y="0.0677" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="parent.license.ask_0_1" ActionTag="-2034474705" Tag="286" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="190.0000" BottomMargin="380.0000" IsCustomSize="True" FontSize="22" LabelText="Vui lòng mở ứng dụng trên thiết bị đã kích hoạt và làm theo các bước dưới đây" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="750.0000" Y="80.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="50.0000" Y="460.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.0588" Y="0.7077" />
                <PreSize X="0.8824" Y="0.1231" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.8301" Y="0.8464" />
            <FileData Type="Normal" Path="mjstory/parent/parent_license_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>