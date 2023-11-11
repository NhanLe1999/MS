<GameFile>
  <PropertyGroup Name="LicenseOldNotice_en" Type="Layer" ID="645920e6-f55a-4fe1-8838-084d3f17892b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="30" Speed="1.0000">
        <Timeline ActionTag="-82831351" Property="Alpha">
          <IntFrame FrameIndex="0" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="204">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Layer" CustomClassName="MSLicenseOldNotice" Tag="77" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="-82831351" Tag="106" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="229" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
          <AbstractNodeData Name="bg" ActionTag="390353759" Tag="93" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="137.0000" RightMargin="137.0000" TopMargin="209.0000" BottomMargin="209.0000" Scale9Enable="True" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="1" Scale9Height="1" ctype="ImageViewObjectData">
            <Size X="750.0000" Y="350.0000" />
            <Children>
              <AbstractNodeData Name="parent.license.ask" ActionTag="-1444317795" Tag="94" IconVisible="False" HorizontalEdge="LeftEdge" VerticalEdge="TopEdge" LeftMargin="25.0000" RightMargin="323.0000" TopMargin="29.9975" BottomMargin="293.0025" FontSize="22" LabelText="Your license key has been changed" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="402.0000" Y="27.0000" />
                <AnchorPoint ScaleY="1.0000" />
                <Position X="25.0000" Y="320.0025" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.0333" Y="0.9143" />
                <PreSize X="0.5360" Y="0.0771" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_close" ActionTag="778405623" CallBackType="Click" CallBackName="onClose" Tag="145" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="726.5000" RightMargin="-23.5000" TopMargin="-23.5000" BottomMargin="326.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="17" Scale9Height="25" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="47.0000" Y="47.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="750.0000" Y="350.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="1.0000" />
                <PreSize X="0.0627" Y="0.1343" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_phone" ActionTag="-1711593400" CallBackType="Click" CallBackName="onSMS" Tag="95" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="265.0000" BottomMargin="25.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="6" BottomEage="6" Scale9OriginX="6" Scale9OriginY="6" Scale9Width="8" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="700.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="via_phone" ActionTag="-1643147127" Tag="96" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="295.0000" RightMargin="295.0000" TopMargin="15.5000" BottomMargin="15.5000" FontSize="24" LabelText="SMS it to" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="110.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="350.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1571" Y="0.4833" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.0000" Y="55.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1571" />
                <PreSize X="0.9333" Y="0.1714" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/parent/parent_notification_item_open_url.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="text" ActionTag="1776181299" Tag="97" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="78.9985" BottomMargin="101.0015" IsCustomSize="True" FontSize="22" LabelText="For security reason, your license key xxxxx has been changed. You can receive your new license key through the email or phone number that you registered with us. " ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="700.0000" Y="170.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="375.0000" Y="271.0015" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="52" G="197" B="237" />
                <PrePosition X="0.5000" Y="0.7743" />
                <PreSize X="0.9333" Y="0.4857" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_mail" ActionTag="1090868753" CallBackType="Click" CallBackName="onMail" Tag="107" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="190.0000" BottomMargin="100.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="6" RightEage="6" TopEage="6" BottomEage="6" Scale9OriginX="6" Scale9OriginY="6" Scale9Width="8" Scale9Height="8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="700.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="via_mail" ActionTag="-2012662143" Tag="108" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="287.0000" RightMargin="287.0000" TopMargin="15.5000" BottomMargin="15.5000" FontSize="24" LabelText="Email it to" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="126.0000" Y="29.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="350.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1800" Y="0.4833" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="375.0000" Y="130.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3714" />
                <PreSize X="0.9333" Y="0.1714" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/parent/parent_notification_item_open_url.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.7324" Y="0.4557" />
            <FileData Type="Normal" Path="mjstory/parent/parent_license_background.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>