<GameFile>
  <PropertyGroup Name="MJPayDetails" Type="Layer" ID="54f5fce7-d5c2-4410-94a7-7a81ff2b2437" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="65" Speed="1.0000" ActivedAnimationName="show">
        <Timeline ActionTag="976033210" Property="Scale">
          <ScaleFrame FrameIndex="5" X="0.2500" Y="0.2500">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="25" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.2500" Y="0.2500">
            <EasingData Type="26" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="976033210" Property="Alpha">
          <IntFrame FrameIndex="5" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="25" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="40" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="689370493" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="35" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="65" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="30">
          <RenderColor A="255" R="128" G="0" B="0" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="35" EndIndex="65">
          <RenderColor A="255" R="0" G="191" B="255" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJPayDetails" Tag="74" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="content" ActionTag="689370493" CallBackName="onBack" Tag="75" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="190" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="img" ActionTag="976033210" Tag="76" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="162.0000" RightMargin="162.0000" TopMargin="34.0000" BottomMargin="34.0000" TouchEnable="True" Scale9Enable="True" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                <Size X="700.0000" Y="700.0000" />
                <Children>
                  <AbstractNodeData Name="text_title" ActionTag="1253253690" VisibleForFrame="False" Tag="196" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="291.0000" RightMargin="291.0000" TopMargin="25.0000" BottomMargin="648.0000" FontSize="20" LabelText="Please click" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="118.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="350.0000" Y="661.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="25" G="166" B="212" />
                    <PrePosition X="0.5000" Y="0.9450" />
                    <PreSize X="0.1686" Y="0.0386" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="28" G="112" B="194" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="button_continue" ActionTag="-1851562838" CallBackType="Click" CallBackName="onContinue" Tag="28" IconVisible="False" LeftMargin="-0.5000" RightMargin="-4.5000" TopMargin="616.0000" BottomMargin="-6.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="705.0000" Y="90.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1" ActionTag="-413985380" Tag="29" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="276.0000" RightMargin="276.0000" TopMargin="25.0000" BottomMargin="25.0000" FontSize="28" LabelText="CONTINUE" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="153.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="352.5000" Y="45.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2170" Y="0.4444" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="28" G="112" B="194" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="352.0000" Y="-6.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5029" Y="-0.0086" />
                    <PreSize X="1.0071" Y="0.1286" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="listview" ActionTag="-2119579373" Tag="225" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="35.0000" BottomMargin="115.0000" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
                    <Size X="675.0000" Y="550.0000" />
                    <Children>
                      <AbstractNodeData Name="text_ms" ActionTag="-29155208" Tag="226" IconVisible="False" BottomMargin="1145.0000" IsCustomSize="True" FontSize="32" LabelText="MONKEY STORIES'S READING PLAN" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="675.0000" Y="75.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="337.5000" Y="1182.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.9693" />
                        <PreSize X="1.0000" Y="0.0615" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="text_price" ActionTag="-1547257795" ZOrder="1" Alpha="232" Tag="227" IconVisible="False" TopMargin="75.0000" BottomMargin="1100.0000" IsCustomSize="True" FontSize="18" LabelText="1499000.00 VND one time, lifetime unlimited access to all stories, games" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="675.0000" Y="45.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="337.5000" Y="1122.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.9201" />
                        <PreSize X="1.0000" Y="0.0369" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="text_details" ActionTag="2014460019" ZOrder="2" Tag="78" IconVisible="False" TopMargin="120.0000" IsCustomSize="True" FontSize="18" LabelText="Includes a 30 day free trial and is just $11.99 month after that&#xA;&#xA;Payment will be charged to iTunes Account at confirmation of purchase&#xA;&#xA;Subscription automatically renews unless auto-renew is turn off at least 24-hours before the end of the current period&#xA;&#xA;Account will be charged $11.99 for renewal within 24-hours prior to the end of the current period&#xA;&#xA;The user may manage subscriptions &amp; may turn off auto-renewal by going to Account Settings after purchase&#xA;&#xA;No cancellation of the current subscription is allowed during active subscription period&#xA;&#xA;Any unused portion of a free trial period will be forfeited when the user purchases a subscription to Monkey Stories!&#xA;&#xA;Privacy Policy&#xA;http://monkeystories.net&#xA;&#xA;Terms of Service&#xA;http://monkeystories.net" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="675.0000" Y="1100.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="337.5000" Y="550.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="127" G="127" B="127" />
                        <PrePosition X="0.5000" Y="0.4508" />
                        <PreSize X="1.0000" Y="0.9016" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="350.0000" Y="390.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5571" />
                    <PreSize X="0.9643" Y="0.7857" />
                    <SingleColor A="255" R="150" G="150" B="255" />
                    <FirstColor A="255" R="150" G="150" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Button_7" ActionTag="-75956858" CallBackType="Click" CallBackName="onBack" Tag="439" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="650.0000" RightMargin="-50.0000" TopMargin="-50.0000" BottomMargin="650.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="100.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_11" ActionTag="-289022642" Tag="440" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="11" RightEage="11" TopEage="12" BottomEage="12" Scale9OriginX="11" Scale9OriginY="12" Scale9Width="53" Scale9Height="51" ctype="ImageViewObjectData">
                        <Size X="75.0000" Y="75.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="50.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.7500" Y="0.7500" />
                        <FileData Type="Normal" Path="mjstory/papgemenucontrol_btclose.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="700.0000" Y="700.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0000" Y="1.0000" />
                    <PreSize X="0.1429" Y="0.1429" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6836" Y="0.9115" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_background.png" Plist="" />
              </AbstractNodeData>
            </Children>
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
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>