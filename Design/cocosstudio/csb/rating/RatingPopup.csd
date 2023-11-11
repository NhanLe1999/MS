<GameFile>
  <PropertyGroup Name="RatingPopup" Type="Layer" ID="16f3fb0c-08a1-491c-b209-b92b0ff532c0" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="RatingPopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-394346387" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="282.0000" RightMargin="282.0000" TopMargin="171.2148" BottomMargin="223.2852" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="78" Scale9Height="37" ctype="ImageViewObjectData">
            <Size X="460.0000" Y="373.5000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="429931610" Tag="49" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="80.4999" RightMargin="80.5001" TopMargin="25.0199" BottomMargin="278.4801" FontSize="30" LabelText="How do you feel about&#xA;VMonkey ? " HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="299.0000" Y="70.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="229.9999" Y="313.4801" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8393" />
                <PreSize X="0.6500" Y="0.1874" />
                <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="line_01" ActionTag="553615885" Tag="51" IconVisible="False" PositionPercentXEnabled="True" TopMargin="119.5000" BottomMargin="250.0000" LeftEage="14" RightEage="14" TopEage="1" BottomEage="1" Scale9OriginX="14" Scale9OriginY="1" Scale9Width="17" Scale9Height="2" ctype="ImageViewObjectData">
                <Size X="460.0000" Y="4.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="230.0000" Y="252.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6747" />
                <PreSize X="1.0000" Y="0.0107" />
                <FileData Type="Normal" Path="rating/line.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="line_02" ActionTag="-174316000" Tag="52" IconVisible="False" PositionPercentXEnabled="True" TopMargin="204.5000" BottomMargin="165.0000" LeftEage="14" RightEage="14" TopEage="1" BottomEage="1" Scale9OriginX="14" Scale9OriginY="1" Scale9Width="17" Scale9Height="2" ctype="ImageViewObjectData">
                <Size X="460.0000" Y="4.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="230.0000" Y="167.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4471" />
                <PreSize X="1.0000" Y="0.0107" />
                <FileData Type="Normal" Path="rating/line.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="line_03" ActionTag="1481109711" Tag="53" IconVisible="False" PositionPercentXEnabled="True" TopMargin="287.5000" BottomMargin="82.0000" LeftEage="14" RightEage="14" TopEage="1" BottomEage="1" Scale9OriginX="14" Scale9OriginY="1" Scale9Width="17" Scale9Height="2" ctype="ImageViewObjectData">
                <Size X="460.0000" Y="4.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="230.0000" Y="84.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.2249" />
                <PreSize X="1.0000" Y="0.0107" />
                <FileData Type="Normal" Path="rating/line.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="happy_button" ActionTag="-1387090661" CallBackType="Click" CallBackName="onHappyClicked" Tag="54" IconVisible="False" PositionPercentXEnabled="True" TopMargin="129.6699" BottomMargin="183.8301" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="460.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="icon" ActionTag="-688413073" Tag="55" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="107.0000" RightMargin="291.0000" TopMargin="-1.0000" BottomMargin="-1.0000" ctype="SpriteObjectData">
                    <Size X="62.0000" Y="62.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="138.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3000" Y="0.5000" />
                    <PreSize X="0.1348" Y="1.0333" />
                    <FileData Type="Normal" Path="rating/happy.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="happy_title" ActionTag="-966196537" Tag="57" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="188.1276" RightMargin="182.8724" TopMargin="12.5000" BottomMargin="12.5000" FontSize="30" LabelText="Happy" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="89.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="188.1276" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.4090" Y="0.5000" />
                    <PreSize X="0.1935" Y="0.5833" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="230.0000" Y="213.8301" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5725" />
                <PreSize X="1.0000" Y="0.1606" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="confused_button" ActionTag="-697176034" CallBackType="Click" CallBackName="onConfusedClicked" Tag="58" IconVisible="False" PositionPercentXEnabled="True" TopMargin="218.6700" BottomMargin="94.8300" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="460.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="icon" ActionTag="-644955577" Tag="59" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="106.0000" RightMargin="290.0000" TopMargin="-2.0000" BottomMargin="-2.0000" ctype="SpriteObjectData">
                    <Size X="64.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="138.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3000" Y="0.5000" />
                    <PreSize X="0.1391" Y="1.0667" />
                    <FileData Type="Normal" Path="rating/confuse.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="confused_title" ActionTag="515780258" Tag="60" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="187.6281" RightMargin="140.3719" TopMargin="12.5000" BottomMargin="12.5000" FontSize="30" LabelText="Confused" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="132.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="187.6281" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.4079" Y="0.5000" />
                    <PreSize X="0.2870" Y="0.5833" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="230.0000" Y="124.8300" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3342" />
                <PreSize X="1.0000" Y="0.1606" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="sad_button" ActionTag="-520100207" CallBackType="Click" CallBackName="onSadClicked" Tag="61" IconVisible="False" PositionPercentXEnabled="True" TopMargin="302.0198" BottomMargin="11.4802" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="460.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="icon" ActionTag="-733697568" Tag="62" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="107.5000" RightMargin="291.5000" TopMargin="-0.5000" BottomMargin="-0.5000" ctype="SpriteObjectData">
                    <Size X="61.0000" Y="61.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="138.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3000" Y="0.5000" />
                    <PreSize X="0.1326" Y="1.0167" />
                    <FileData Type="Normal" Path="rating/sad.png" Plist="" />
                    <BlendFunc Src="1" Dst="771" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="sad_title" ActionTag="259885299" Tag="63" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="187.6546" RightMargin="219.3454" TopMargin="12.5000" BottomMargin="12.5000" FontSize="30" LabelText="Sad" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="53.0000" Y="35.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="187.6546" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.4079" Y="0.5000" />
                    <PreSize X="0.1152" Y="0.5833" />
                    <FontResource Type="Normal" Path="fonts/Roboto-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="230.0000" Y="41.4802" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1111" />
                <PreSize X="1.0000" Y="0.1606" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="410.0352" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5339" />
            <PreSize X="0.4492" Y="0.4863" />
            <FileData Type="Normal" Path="rating/rating_bg.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="cancel_button" ActionTag="1410488007" CallBackType="Click" CallBackName="onCancel" Tag="64" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="282.0000" RightMargin="282.0000" TopMargin="556.5480" BottomMargin="136.4520" TouchEnable="True" FontSize="30" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="78" Scale9Height="57" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="460.0000" Y="75.0000" />
            <Children>
              <AbstractNodeData Name="cancel_txt" ActionTag="268407080" Tag="65" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="183.5000" RightMargin="183.5000" TopMargin="20.0000" BottomMargin="20.0000" FontSize="30" LabelText="Cancel" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="93.0000" Y="35.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="230.0000" Y="37.5000" />
                <Scale ScaleX="1.0000" ScaleY="-1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.2022" Y="0.4667" />
                <FontResource Type="Normal" Path="fonts/Roboto-Regular.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="173.9520" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.2265" />
            <PreSize X="0.4492" Y="0.0977" />
            <FontResource Type="Default" Path="" Plist="" />
            <TextColor A="255" R="0" G="0" B="0" />
            <NormalFileData Type="Normal" Path="rating/rating_bg.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="cancel_bg" Visible="False" ActionTag="2042139217" VisibleForFrame="False" Tag="47" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="282.0000" RightMargin="282.0000" TopMargin="556.5480" BottomMargin="136.4520" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="78" Scale9Height="37" ctype="ImageViewObjectData">
            <Size X="460.0000" Y="75.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="173.9520" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.2265" />
            <PreSize X="0.4492" Y="0.0977" />
            <FileData Type="Normal" Path="rating/rating_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>