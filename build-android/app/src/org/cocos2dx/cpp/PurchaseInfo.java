package org.cocos2dx.cpp;

public class PurchaseInfo
{
    private String  _productId;

    private String  _purchaseToken;

    private Long    _purchaseTime;

    public PurchaseInfo(String _productId, String _purchaseToken) {
        this._productId = _productId;
        this._purchaseToken = _purchaseToken;
    }

    public PurchaseInfo(String _productId, String _purchaseToken, Long _purchaseTime) {
        this._productId = _productId;
        this._purchaseToken = _purchaseToken;
        this._purchaseTime = _purchaseTime;
    }

    public String getProductId() {
        return _productId;
    }

    public void setProductId(String _productId) {
        this._productId = _productId;
    }

    public String getPurchaseToken() {
        return _purchaseToken;
    }

    public void setPurchaseToken(String _purchaseToken) {
        this._purchaseToken = _purchaseToken;
    }

    public Long getPurchaseTime() {
        return _purchaseTime;
    }

    public void setPurchaseTime(Long _purchaseTime) {
        this._purchaseTime = _purchaseTime;
    }

    @Override
    public boolean equals(Object other) {

        if (!(other instanceof PurchaseInfo)) {
            return false;
        }

        PurchaseInfo that = (PurchaseInfo) other;

        return this._productId.equals(that._productId) &&
                this._purchaseTime.equals(that._purchaseTime) &&
                this._purchaseToken.equals(that._purchaseToken);
    }
}
