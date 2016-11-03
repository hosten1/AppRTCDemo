/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#import <Foundation/Foundation.h>

@class RTCConfiguration;
@class RTCDataChannel;
@class RTCDataChannelConfiguration;
@class RTCIceCandidate;
@class RTCMediaConstraints;
@class RTCMediaStream;
@class RTCMediaStreamTrack;
@class RTCPeerConnectionFactory;
@class RTCSessionDescription;
@class RTCStatsReport;

NS_ASSUME_NONNULL_BEGIN

extern NSString * const kRTCPeerConnectionErrorDomain;
extern int const kRTCSessionDescriptionErrorCode;

/** Represents the signaling state of the peer connection. */
typedef NS_ENUM(NSInteger, RTCSignalingState) {
  RTCSignalingStateStable,
  RTCSignalingStateHaveLocalOffer,
  RTCSignalingStateHaveLocalPrAnswer,
  RTCSignalingStateHaveRemoteOffer,
  RTCSignalingStateHaveRemotePrAnswer,
  RTCSignalingStateClosed,
};

/** Represents the ice connection state of the peer connection. */
typedef NS_ENUM(NSInteger, RTCIceConnectionState) {
  RTCIceConnectionStateNew,
  RTCIceConnectionStateChecking,
  RTCIceConnectionStateConnected,
  RTCIceConnectionStateCompleted,
  RTCIceConnectionStateFailed,
  RTCIceConnectionStateDisconnected,
  RTCIceConnectionStateClosed,
  RTCIceConnectionStateMax,
};

/** Represents the ice gathering state of the peer connection. */
typedef NS_ENUM(NSInteger, RTCIceGatheringState) {
  RTCIceGatheringStateNew,
  RTCIceGatheringStateGathering,
  RTCIceGatheringStateComplete,
};

/** Represents the stats output level. */
typedef NS_ENUM(NSInteger, RTCStatsOutputLevel) {
  RTCStatsOutputLevelStandard,
  RTCStatsOutputLevelDebug,
};

@class RTCPeerConnection;

@protocol RTCPeerConnectionDelegate <NSObject>

/** Called when the SignalingState changed. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
    didChangeSignalingState:(RTCSignalingState)stateChanged;

/** Called when media is received on a new stream from remote peer. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
          didAddStream:(RTCMediaStream *)stream;

/** Called when a remote peer closes a stream. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
       didRemoveStream:(RTCMediaStream *)stream;

/** Called when negotiation is needed, for example ICE has restarted. */
- (void)peerConnectionShouldNegotiate:(RTCPeerConnection *)peerConnection;

/** Called any time the IceConnectionState changes. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
    didChangeIceConnectionState:(RTCIceConnectionState)newState;

/** Called any time the IceGatheringState changes. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
    didChangeIceGatheringState:(RTCIceGatheringState)newState;

/** New ice candidate has been found. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
    didGenerateIceCandidate:(RTCIceCandidate *)candidate;

/** New data channel has been opened. */
- (void)peerConnection:(RTCPeerConnection *)peerConnection
    didOpenDataChannel:(RTCDataChannel *)dataChannel;

@end


@interface RTCPeerConnection : NSObject

/** The object that will be notifed about events such as state changes and
 *  streams being added or removed.
 */
@property(nonatomic, weak) id<RTCPeerConnectionDelegate> delegate;
@property(nonatomic, readonly) NSArray *localStreams;
@property(nonatomic, readonly, nullable)
    RTCSessionDescription *localDescription;
@property(nonatomic, readonly, nullable)
    RTCSessionDescription *remoteDescription;
@property(nonatomic, readonly) RTCSignalingState signalingState;
@property(nonatomic, readonly) RTCIceConnectionState iceConnectionState;
@property(nonatomic, readonly) RTCIceGatheringState iceGatheringState;

- (instancetype)init NS_UNAVAILABLE;

/** Initialize an RTCPeerConnection with a configuration, constraints, and
 *  delegate.
 */
- (instancetype)initWithFactory:(RTCPeerConnectionFactory *)factory
                  configuration:(RTCConfiguration *)configuration
                    constraints:(RTCMediaConstraints *)constraints
                       delegate:(id<RTCPeerConnectionDelegate>)delegate
    NS_DESIGNATED_INITIALIZER;

/** Terminate all media and close the transport. */
- (void)close;

/** Provide a remote candidate to the ICE Agent. */
- (void)addIceCandidate:(RTCIceCandidate *)candidate;

/** Add a new media stream to be sent on this peer connection. */
- (void)addStream:(RTCMediaStream *)stream;

/** Remove the given media stream from this peer connection. */
- (void)removeStream:(RTCMediaStream *)stream;

/** Generate an SDP offer. */
- (void)offerForConstraints:(RTCMediaConstraints *)constraints
          completionHandler:(void (^)(RTCSessionDescription *sdp,
                                      NSError *error))completionHandler;

/** Generate an SDP answer. */
- (void)answerForConstraints:(RTCMediaConstraints *)constraints
           completionHandler:(void (^)(RTCSessionDescription *sdp,
                                       NSError *error))completionHandler;

/** Apply the supplied RTCSessionDescription as the local description. */
- (void)setLocalDescription:(RTCSessionDescription *)sdp
          completionHandler:(void (^)(NSError *error))completionHandler;

/** Apply the supplied RTCSessionDescription as the remote description. */
- (void)setRemoteDescription:(RTCSessionDescription *)sdp
           completionHandler:(void (^)(NSError *error))completionHandler;

@end

@interface RTCPeerConnection (DataChannel)

/** Create a new data channel with the given label and configuration. */
- (RTCDataChannel *)dataChannelForLabel:(NSString *)label
    configuration:(RTCDataChannelConfiguration *)configuration;

@end

@interface RTCPeerConnection (Stats)

/** Gather stats for the given RTCMediaStreamTrack. If |mediaStreamTrack| is nil
 *  statistics are gathered for all tracks.
 */
- (void)statsForTrack:
    (nullable RTCMediaStreamTrack *)mediaStreamTrack
     statsOutputLevel:(RTCStatsOutputLevel)statsOutputLevel
    completionHandler:
    (void (^)(NSArray<RTCStatsReport *> *stats))completionHandler;

@end

NS_ASSUME_NONNULL_END
